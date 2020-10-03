#include<stdio.h>
#include<conio.h>
#include<math.h>


float * lambda_RGB(float Wavelength)
{
	float Red, Green, Blue, factor;

float Gamma = 0.80;
 float IntensityMax = 255;
float rgb[3] ;

	if((Wavelength >= 380) && (Wavelength < 440)) {
        Red = -(Wavelength - 440) / (440 - 380);
        Green = 0.0;
        Blue = 1.0;
    } else if((Wavelength >= 440) && (Wavelength < 490)) {
        Red = 0.0;
        Green = (Wavelength - 440) / (490 - 440);
        Blue = 1.0;
    } else if((Wavelength >= 490) && (Wavelength < 510)) {
        Red = 0.0;
        Green = 1.0;
        Blue = -(Wavelength - 510) / (510 - 490);
    } else if((Wavelength >= 510) && (Wavelength < 580)) {
        Red = (Wavelength - 510) / (580 - 510);
        Green = 1.0;
        Blue = 0.0;
    } else if((Wavelength >= 580) && (Wavelength < 645)) {
        Red = 1.0;
        Green = -(Wavelength - 645) / (645 - 580);
        Blue = 0.0;
    } else if((Wavelength >= 645) && (Wavelength < 781)) {
        Red = 1.0;
        Green = 0.0;
        Blue = 0.0;
    } else {
        Red = 0.0;
        Green = 0.0;
        Blue = 0.0;
    }
    
      if((Wavelength >= 380) && (Wavelength < 420)) {
        factor = 0.3 + 0.7 * (Wavelength - 380) / (420 - 380);
    } else if((Wavelength >= 420) && (Wavelength < 701)) {
        factor = 1.0;
    } else if((Wavelength >= 701) && (Wavelength < 781)) {
        factor = 0.3 + 0.7 * (780 - Wavelength) / (780 - 700);
    } else {
        factor = 0.0;
    }


   

    // Don't want 0^x = 1 for x <> 0
    rgb[0] = Red == 0.0 ? 0 : round(IntensityMax * pow(Red * factor, Gamma));
    rgb[1] = Green == 0.0 ? 0 : round(IntensityMax *pow(Green * factor, Gamma));
    rgb[2] = Blue == 0.0 ? 0 : round(IntensityMax * pow(Blue * factor, Gamma));

    return rgb;
		
}
float max(float r, float g, float b)
{
	float max_val = r;
	if(max_val < g)
	{
		max_val = g;
	}
	if (max_val < b)
	{
		max_val = b;
	}
	return max_val;
}

float min(float r, float g, float b)
{
	float min_val = r;
	if(min_val > g)
	{
		min_val = g;
	}
	if (min_val > b)
	{
		min_val = b;
	}
	return min_val;
}
float RGB_to_lambda(float r, float g, float b)
{
	float max_val, min_val, h, s, v, wavelength;
	r = r / 255.0;
    g = g /255.0;
    b = b / 255.0;
   
  
   
    max_val = max(r, g, b);
     printf("max:%f\n", max_val);
    min_val = min(r, g, b);
printf("min:%f", min_val);
     
  if(max_val == 0)
       {
		 s = 0;
        h = 0;
      

    }

  else if ((max_val - min_val) == 0.0 )
     {
	 
        s = 0;
        h = 0;
       
    }
     else
     {
	
        s = (max_val - min_val) / max_val;
        if (max_val == r)
        {
		
            h = 60 * ((g - b) / (max_val - min_val)) + 0;
       
        }
         else if (max_val == g )
         {
		 
            h = 60 * ((b - r) / (max_val - min_val)) + 120;
            
        }
        
        else
        {
		
            h = 60 * ((r - g) / (max_val - min_val)) + 240;
        }
}

if(h < 0)
{

        h = h + 360.0;
    }
    
    
    
   //  h = h / 2;
    s = s * 255.0;
    v = v * 255.0;
    printf("\n H value is %f", h);
    
   
     wavelength = 620.0 - (250.0 / 270.0 )* h;
   
    printf("New Lambda ..........: %f", wavelength);
    return wavelength;

}


float lambda_to_new_lambda(float lambda, float V_radical, float c)
{
	float new_lambda;
	float beta =  V_radical/c;
	new_lambda = lambda * (sqrt(1 - beta))/(sqrt(1 + beta));
		return new_lambda;
}
int main(void)
{
	float V_radial, R, G, B, lambda, change_lambda, R1, G1, B1, R2, G2, B2;
	int forward;
	float IR_wavelength;
	float IR_shift;
	float UV_wavelength;
	float UV_shift;
	float new_lambda;
	float c = 300000000.0;
	float *color;
	printf("Enter the Current speed");
	scanf(" %f", &V_radial);
	printf("Enter the RGB value");
	scanf(" %f%f%f", &R, &G, &B);
	
	//printf("Enter forward(1) or backward(0)");
	//scanf("%d", &forward);
	
	//printf("Enter the IR waveLength");
	//scanf("%f", &IR_wavelength);
	
	//printf("Enter the UV waveLength");
	//scanf("%f", &UV_wavelength);
	
	
	lambda = RGB_to_lambda(R, G, B);
    
    //UV_shift = IT_to_visible(V_radial, c, forward, UV_wavelength);
	//printf("The UV shift is:%f \n", UV_shift);
	//	if((UV_shift>=350) && (UV_shift<= 700))
	//{
	//	color = lambda_RGB(UV_shift);
	//	R2 = color[0];
	//	G2 = color[1];
	//	B2 = color[2];
	//}
//	else
//	{
//		printf("UV shift lies outside Visible range\n");
//	}
	
/**	IR_shift = IT_to_visible(V_radial, c, forward, IR_wavelength);
	printf("The IR shift is:%f \n", IR_shift);
	if((IR_shift>=350) && (IR_shift<= 700))
	{
		color = lambda_RGB(IR_shift);
		R1 = color[0];
		G1 = color[1];
		B1 = color[2];
	}
	else
	{
		printf("IR shift lies outside Visible range\n");
	}
	
	**/
	if ((V_radial/c)>0.01)
	{
		
		new_lambda = lambda_to_new_lambda(lambda, V_radial, c);
		color = lambda_RGB(new_lambda);
		R = color[0];
		G = color[1];
		B = color[2];

	if (new_lambda >700)
		{
			printf("Light Ray is in IR Range \n ");
		}
		else if (new_lambda < 350)
		{
			printf("Light Ray in UV Range \n");
		}
		else
		{
		

            printf("R:%f \n G:%f \n B:%f \n", R, G, B);
		}
	}
	else
	{
			printf("No Remarkable change in Wavelength!! \n");
	}
	
}
