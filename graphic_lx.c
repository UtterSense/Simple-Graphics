/*-----------------------------------------------------------------------------
   AUTHOR:            M.J. McGetrick
                      (c) Copyright 2020 M.J. McGetrick
   DATE:
   DESCRIPTION:       Graphic_lx
                      Suite of functions for simple graph plotting using
                      the Linux Turbo C graphics library.
    
------------------------------------------------------------------------------*/
#include "graphic_lx.h"


#include <stdio.h>
#include <math.h>
#include <string.h>


//---------------------------------------------------------------------------
  void Graphic_Init(int xFormSize,int yFormSize,int pMode,int logMode,
               char *xlab, char *ylab, char *gphtitle,
               float yMin, float yMax, float xMin, float xMax,
               int xInt, int yInt, bool xGrad, bool yGrad,
               bool grid, int gInt,
               bool xshow,bool yshow,bool ori, bool bitmap,
               int monobcolor,char *audfile, char *audInifile,
               int chans,int samrate, int bps)
{
   
   //Initialise Turbo C Graphics Library
   int gd = DETECT,gm;
   initgraph(&gd,&gm,NULL);
   
      
   X_FORM_SIZE = xFormSize;
   Y_FORM_SIZE = yFormSize;
   
     
  m_bAnimated = false;
  m_bResize = false;
  
   //Set scales ranges 
   ymin1 = yMin;
   ymax1 = yMax;
   xmin = xMin;
   xmax = xMax;


   //Save linear definitions (in case transformed to log values):
   xminLinear = xmin;
   xmaxLinear = xmax;

   //Get flag for plotting harmonics on frequency spectrum
   m_bHarmonic = false;

   //Set audio player members:
   strcpy(m_szAudioIniFile,audInifile);
   strcpy(m_sFile,audfile);
   m_iNumChannels = chans;
   m_iSampleRate = samrate;
   m_iBitsPerSample = bps;

   
   //Check for bitmap option:
   Bitmap = bitmap;

   //Define plot mode:
   plotMode = pMode;  //0:line, 1:pixel, 2:circle, 3:rectangle
   logPlotMode = logMode;  //0: default - no log plotting
                          //1: log plot: x axis
                          //2: log plot: y axis
                          //3: log plot: x and y axis


   
   image = 1;

   
   xincrement = 1; //Default value for pixel plot advances
   cirSize = 5;    //Initialise circle size for circle plots
   
   
   setLinePlotFlag(0);

   
   //Set  axis numbering defaults:
   showXAxisNums = xshow;
   showYAxisNums = yshow;

   //Set title:
   strcpy(Title,gphtitle);
   //Set axis labels:
   strcpy(xLabel,xlab);
   strcpy(yLabel,ylab);

   //Set flags for graduations:
   xgrad = xGrad;
   ygrad = yGrad;
   xnoDivs = xInt;
   ynoDivs = yInt;

   showGrid = grid;
   showXGradLines = xshow;
   showYGradLines = yshow;

   if(logPlotMode == 1)
   {
      xmin = log10(xmin);
      if(xmax!= 0)
         xmax = log10(xmax);
   }
   if(logPlotMode == 2)
   {
      ymin1 = log10(ymin1);
      if(ymax1!= 0)
         ymax1 = log10(ymax1);
   }
   if(logPlotMode == 3)
   {
      ymin1 = log10(ymin1);
      if(ymax1!= 0)
         ymax1 = log10(ymax1);
      xmin = log10(xmin);
      if(xmax!= 0)
         xmax = log10(xmax);
   }
   
   
   //Set up test rectangle:
   //rectangle(0,0, X_FORM_SIZE,Y_FORM_SIZE);
   //Initialise background:
   setbkcolor(WHITE);
   
  //Set up display graphic area:  -this is the outer rectangle for entire graph (plot + labels)
  setupGraphSize();


   
   //Color background for mono graph:
   setMonoGraphBackground(monobcolor);
   m_cMonoBackCol = monobcolor;

   
   setupWindow(image,xmargin,ymargin,X_FORM_SIZE-xmargin,
               Y_FORM_SIZE-ymargin,xLabel,yLabel);


    
  counter1 = 0.0;
  strcpy(xfield,"5.1");
  strcpy(yfield,"5.1");
  //Draw graduations:
  if(showGrid)
  {
      if(m_cMonoBackCol == 0)
      {
         graduations(15);
      }
      if(m_cMonoBackCol == 15)
      {
         graduations(0);
      }   
  }
  
      
      
  //Draw axis scales:
  if(!Bitmap) axisScales();
  
  //Draw origin line:
  show_ori_line = ori;
  
  if(show_ori_line)
  {
      if(m_cMonoBackCol == 0)
      {
         setcolor(15);
      }
      if(m_cMonoBackCol == 15)
      {
         setcolor(0);
      }   
      drawOriginLine();
  }    
       

}

//---------------------------------------------------------------------------
void setupGraphSize()
{
      //Set up outer rectangle window:
      
      xmargin = 30;
      ymargin = 35;

      left = xmargin;
      left1 = left;
      top = ymargin;
      top1 = top;
      right = X_FORM_SIZE-xmargin;
      right1 = right;
      bottom = Y_FORM_SIZE-ymargin;
      bottom1 = bottom;
       


}//setupGraphSize

//---------------------------------------------------------------------------
void setMonoGraphBackground(int color)
{
   
   //Set draw color:
   if(color == 0)
   {
      setcolor(color);
      rectangle(xmargin,ymargin,X_FORM_SIZE-xmargin,Y_FORM_SIZE-ymargin);
      //Set fill color:
      floodfill(xmargin+10, ymargin+10,color);
   }
   if(color == 15)
   {    
      setcolor(0);   //Provide black border
      rectangle(xmargin,ymargin,X_FORM_SIZE-xmargin,Y_FORM_SIZE-ymargin);
      //Set fill color:
      setcolor(color);
      floodfill(xmargin+10, ymargin+10,color);
    }

}//setMonoGraphBackGround

//---------------------------------------------------------------------------
void setupWindow(int image,int left,int top,int right,
                             int bottom, char *xlabel, char *ylabel)
{
   
   //Print title and axis labels:
   title(Title);
   xAxisText(image,xlabel);
   yAxisText(image,ylabel);


}//setupWindow


//---------------------------------------------------------------------------
void title(char *text)
{
   
   int len = X_FORM_SIZE;
   int txtWidth = textwidth(text);
   int centralPos = (len-txtWidth)/2;
   int horPos = top-25;
   
   //Print graph title 
   setfontcolor(0);
   outtextxy(centralPos,horPos,text);

}//title

//---------------------------------------------------------------------------
void xAxisText(int image,char *text)
{

   int len = X_FORM_SIZE;
   int txtWidth = textwidth(text);
   int centralPos = (len-txtWidth)/2;
   int horPos = bottom1+18;
   
   //Print x axis label 
   setfontcolor(0);
   outtextxy(centralPos,horPos,text);



}//xAxisText

//---------------------------------------------------------------------------
void yAxisText(int image,char *text)
{
      int len = bottom1-top1;
      int txtHeight = textheight(text);
      int verlen = txtHeight*strlen(text);
      int centralPos = (len-verlen)/2;
      int pos = centralPos + ymargin;
      
      //Print y axis label
      char str[2];
      str[1] = '\0';
      setfontcolor(0);
      for(int i=0;i<strlen(text);i++)
      {
         str[0] = text[i];
         
         outtextxy(1,pos,str);
         pos = pos+15;
      }//i
      
}//yAxisText



//---------------------------------------------------------------------------
void tagOrigin(int image,int left,int posOrig)
{

   //Mark horizontal position of origin:
   setcolor(0);
   moveto(left-5,C1);
   lineto(left,C1);

}//tagOrigin

//---------------------------------------------------------------------------
void graduations(int color)
{
   int Image = 1;   //Set for mono graph plot:
   
   setcolor(color);
   
   if(showXGradLines)
   {
      //Obtain vertical graduations:
      if(logPlotMode == 1 || logPlotMode == 3)
      {
         //log graduations:
         for(int i=xmin;i<=xmax-1; i+=1)
         {
             int x = positionPixel(Image,xmin,xmax,i,1);
             pixelVerLine(x,4);
             for(int n=2;n<10;n++)
             {
               //float pos = log10( n*pow10(i) );
               float pos = log10( n*pow(10,i) );
               x = positionPixel(Image,xmin,xmax,pos,1);
               pixelVerLine(x,4);
             }//n
         }//i

      }
      else
      {  //linear graduations
         if(xgrad)
         {
            float interval = (xmax-xmin)/xnoDivs;
            float pos = xmin;
            for(int i=0;i<=xnoDivs;i++)
            {
               int x = positionPixel(Image,xmin,xmax,pos,1);
               pixelVerLine(x,4);
               pos += interval;
            }//i
         }//if

      }
   }//if(showXGradLines)
   
   if(showYGradLines)
   {
       //Obtain horizontal graduations:
      if(logPlotMode == 2 || logPlotMode == 3)
      {
         for(int i=ymin1;i<=ymax1-1;i+=1)
         {
            int y = positionPixel(Image,ymin1,ymax1,i,0);
            pixelHorLine(y,3);
            for(int n=2;n<10;n++)
            {
               //float pos = log10( n*pow10(i) );
               float pos = log10( n*pow(10,i) );
               y = positionPixel(Image,ymin1,ymax1,pos,0);
               pixelHorLine(y,3);
            }//n
         }//i
      }
      else
      {
         if(ygrad)
         {
            float interval = (ymax1-ymin1)/ynoDivs;
            float pos = ymin1;
            for(int i=0;i<=ynoDivs;i++)
            {
               int y = positionPixel(Image,ymin1,ymax1,pos,0);
               pixelHorLine(y,3);
               pos += interval;
            }//i
         }//if
      }
   }//if(showYGradLines

}//graduations

//---------------------------------------------------------------------------
void plotSig(float ySig, float xSig,int color)
{
   if(logPlotMode == 1) xSig = log10(xSig);
   if(logPlotMode == 2) ySig = log10(ySig);
   if(logPlotMode == 3)
   {
      xSig = log10(xSig);
      ySig = log10(ySig);
   }
   int ypixel = positionPixel(1,ymin1,ymax1,ySig,0);
   int xpixel = positionPixel(1,xmin,xmax,xSig,1);
   
   setcolor(color);
   if(plotMode == 2)
   {
      //Image->Canvas->Ellipse(xpixel,ypixel,xpixel+cirSize,
      //                        ypixel+cirSize);
      circle(xpixel,ypixel,cirSize);                        
   }
   if(plotMode == 3)
   {
      rectangle(xpixel,ypixel,xpixel+cirSize,ypixel+cirSize);                        
   }
   if(plotMode == 1)
   {
      //int pixelSize = 1;
      moveto(xpixel,ypixel);
      lineto(xpixel,ypixel+1);
   }
   if(plotMode == 0)
   {
      if(lineFlag>0)
      {
         moveto(sigStore1,sigStore2);
         lineto(xpixel,ypixel);
      }
      sigStore1 = xpixel;
      sigStore2 = ypixel;
      lineFlag = 1;
   }
   if(ySig>ymaxVal1) ymaxVal1 = ySig;

}//plotSig  (for single screen plotting)



//---------------------------------------------------------------------------
int positionPixel(int image,float ymin,float ymax,float sig,
                              int pixelMode)
{
   int X,y;
   float R = ymax/(ymax-ymin);
   if(ymax==0) R = ymin/(ymin-ymax);
   if(pixelMode == 0)
   {                    //positioning along y axis
      if(image == 1) X = bottom1 - top1;
      else if(image == 2)
        X = bottom2 - top2;
      if(ymax==0) y = (fabs(sig)/(-ymin))*R*X;
      else
         y = (fabs(sig)/ymax)*R*X;
      if(sig>=0.0)
      {
         if(ymax != 0) y = (R*X)-y;
      }
      else
      {
         if(ymax != 0)
            y = y+(R*X);
      }
      y += ymargin;
   }//if
   if(pixelMode == 1)
   {                //positioning along x axis
      float R = ymax/(ymax-ymin);
      //if(ymax==0) R = ymin/(ymin-ymax);
      R = 1-R;
      if(image == 1) X = right1 - left1;
      else if(image == 2)
         X = right2 - left2;
      float orig = R*X;
      if(sig>=0.0)
      {
         if(ymax==0) y = (fabs(sig)/(-ymin))*(X-orig);
         else
            y = orig + (fabs(sig)/ymax)*(X-orig);
      }
      if(sig<0.0)
      {
         y = orig - (fabs(sig)/fabs(ymin))*orig;
      }
      y+= xmargin;
   }

   return y;

}//positionPixel


void plotVerLine(float ySig,float xSig)
{

   //This routine plots a vertical line (along +yaxis) of given height at x position specified

   //if(logPlotMode == 1) xSig = log10(xSig);
   if(logPlotMode == 2) ySig = log10(ySig);
   if(logPlotMode == 3)
   {
      xSig = log10(xSig);
      ySig = log10(ySig);
   }

   int yminpixel = positionPixel(1,ymin1,ymax1,0.0,0);
   int ymaxpixel = positionPixel(1,ymin1,ymax1,ySig,0);
   int xpixel = positionPixel(1,xmin,xmax,xSig,1);

   setcolor(15);
   moveto(xpixel,yminpixel);
   lineto(xpixel,ymaxpixel);
   

}//plotVerLine



//---------------------------------------------------------------------------
void pixelVerLine(int x, int pixelIncr)
{
   for(int j=top;j<=bottom;j+=pixelIncr)
   {
      moveto(x,j);
      lineto(x,j+1);
   }//j

}//pixelVerLine


//---------------------------------------------------------------------------
void pixelHorLine(int y, int pixelIncr)
{
   for(int j=left;j<=right;j+=pixelIncr)
   {
      moveto(j,y);
      lineto(j+1,y);
   }//j

}//pixelVerLine


//---------------------------------------------------------------------------
void axisScales()
{ 
   xAxisScale(); 
   yAxisScale(); //Only for mono-trace
}

//---------------------------------------------------------------------------
void xAxisScale()
{
   if(showXAxisNums)
   {
      char numtext[3], powtext[3];

      sprintf(numtext,"%d",10);
      int horPos = (bottom-top)+14;

      setfontcolor(0);
      //Obtain x axis scale:
      if(logPlotMode == 1 || logPlotMode == 3)
      {
         for(int i=xmin;i<=xmax;i+=1)
         {
            int x = positionPixel(1,xmin,xmax,i,1);
            sprintf(powtext,"%d",i);
            x = x-30;
            outtextxy(x+15,horPos+25,numtext);
            outtextxy(x+28,horPos+20,powtext);
         }//i

      }
      else
      {
         char floatnumtext[6], field[6];
         char *s1 = "%", *s2 = "f";
         strcpy(field,s1);
         strcat(field,xfield);
         strcat(field,s2);

         float interval = (xmax-xmin)/xnoDivs;
         float pos = xmin;
         for(int i=0;i<=xnoDivs;i++)
         {
            int x = positionPixel(1,xmin,xmax,pos,1);
            sprintf(floatnumtext,field,pos);
            x = x-30;
            outtextxy(x+15,horPos+25,floatnumtext);
            pos += interval;

         }//i

      }
   }//if(ShowXAxisNums)

}//xAxisScale

//---------------------------------------------------------------------------
void yAxisScale()
{

   if(showYAxisNums)
   {
      char numtext[3], powtext[3];

      sprintf(numtext,"%d",10);
      int verPos = left-85;

      setfontcolor(0);
      //Obtain y axis scale:
      if(logPlotMode == 2 || logPlotMode == 3)
      {
         for(int i = ymin1;i<=ymax1;i+=1)
         {
            int y = positionPixel(1,ymin1,ymax1,i,0);
            sprintf(powtext,"%d",i);
            outtextxy(verPos+60,y-5,numtext);
            outtextxy(verPos+75,y-10,powtext);
         }//i
      }
      else
      {
         char floatnumtext[6], field[6];
         char *s1 = "%", *s2 = "f";
         strcpy(field,s1);
         strcat(field,yfield);
         strcat(field,s2);

         float interval = (ymax1-ymin1)/ynoDivs;
         float pos = ymin1;
         for(int i=0;i<=ynoDivs;i++)
         {
            sprintf(floatnumtext,field,pos);
            int y = positionPixel(1,ymin1,ymax1,pos,0);
            outtextxy(verPos+55,y-6,floatnumtext);
            pos += interval;
         }//i

      }
   }//if(showYAxisNums)

}//yAxisScale


//---------------------------------------------------------------------------
void drawOriginLine()
{
    
  if(logPlotMode == 0 || logPlotMode == 1)
  {
      C1 = ( ymax1/(ymax1-ymin1) )* (bottom-top);
      C1 = C1 + ymargin;  //Add margin offset
      moveto(left,C1);
      lineto(right,C1);
      tagOrigin(image,left,C1);
  }
  
}//drawOriginLine

//---------------------------------------------------------------------------
void setupGrid(int xInterval,int yInterval)
{
   verLines(xInterval);
   horLines(yInterval);

}//setupGrid

//---------------------------------------------------------------------------
void verLines(int xInterval)
{
   setcolor(15);
   for(int i=left1;i<=right1;i+=xInterval)
   {
      moveto(i,top1);
      lineto(i,bottom1);
   }//i
   

}//verLines

//---------------------------------------------------------------------------
void horLines(int yInterval)
{
   //Produce horizontal lines at yInterval on each side of origin:
   setcolor(15);
   for(int i=C1;i>=top1;i-=yInterval)
   {
      moveto(left1,i);
      lineto(right1,i);
   }//i

   for(int i=C1;i<=bottom1;i+=yInterval)
   {
      moveto(left1,i);
      lineto(right1,i);
   }//i


}//horLines


void text(int image,float x,float y,char *caption)
{
   
   
   int ypixel = positionPixel(image,ymin1,ymax1,y,0);
   int xpixel = positionPixel(image,xmin,xmax,x,1);
   setfontcolor(15);
   outtextxy(xpixel,ypixel,caption);
   
   
}//text

//---------------------------------------------------------------------------
void setLinePlotFlag(int flg)
{
  //Only use with single trace
  lineFlag = flg;
}

//---------------------------------------------------------------------------
void changePlotMode(int p) 
{
   plotMode = p;
}

//---------------------------------------------------------------------------
void changeLogPlotMode(int p) 
{
   logPlotMode = p;
}

//---------------------------------------------------------------------------
void xPlotIncr(int xincr)
{
   xincrement = xincr;
}

//---------------------------------------------------------------------------
void circleSize(int cSize)
{
   cirSize = cSize;
}


void legendCaptions(struct leg_captions l_caps)
{
   
   int left = 50;
   int top = 50;
   
   //Write legend border:
   setcolor(4);
   rectangle(left-5,top-5,left+120,top+80);
   //Write captions:
   setfontcolor(15);   
   //outtextxy(left,top,cap1);
   //outtextxy(left,top+20,cap2);
   outtextxy(left,top,l_caps.vmax_caption);
   outtextxy(left,top+20,l_caps.vmin_caption);
   outtextxy(left,top+40,l_caps.vavg_caption);
   outtextxy(left,top+60,l_caps.vpp_caption);


}//legendCaptions

//---------------------------------------------------------------------------
void legend(struct leg_data l_data,bool refresh)
{
   
   //NB: If working for non animating graph, refresh is set to zero
   
   char cap1[30], cap2[30], cap3[30], cap4[30];;

     
   sprintf(cap1,"%5.2f",l_data.vmax);
   sprintf(cap2,"%5.2f",l_data.vmin);
   sprintf(cap3,"%5.2f",l_data.vavg);
   sprintf(cap4,"%5.2f",l_data.vpp);
      
   int left = 50;
   int top = 50;

   
   /*
   //Refresh background before writing current data (needed for animated graphics):
   int color = m_cMonoBackCol; //Background color
   setcolor(color);
   //rectangle(xmargin,ymargin,X_FORM_SIZE-xmargin,Y_FORM_SIZE-ymargin);
   rectangle(left+55,top,left+93,top+32);
   //Set fill color:
   floodfill(left+55+1, top+32-1,color);   
    */
    
    if(refresh)  //Repaint to background previous value  
    {
       setfontcolor(m_cMonoBackCol);
       
    }  
    else
    {       
          setfontcolor(3); //10-light green; 5-Magenta
    }      
      
   //Output vmax
   outtextxy(left+50,top,cap1);  
   //Output vmin
   outtextxy(left+50,top+20,cap2);
   //Ouput vavg:
   outtextxy(left+50,top+40,cap3);
   //Output vpp:
   outtextxy(left+50,top+60,cap4);     
   

}//legend



//---------------------------------------------------------------------------
void axisDefaults(int xdivs,int ydivs, char *xspec,
                              char *yspec)
{
   xnoDivs = xdivs;   //Redefine no. scale divs. along x-axis
   ynoDivs = ydivs;   //Redefine no. scale divs. along y-axis
   strcpy(xfield,xspec);  //Redefine field specification: x scaling
   strcpy(yfield,yspec);  //Redefine field specification: y scaling

}//axisDefaults

//---------------------------------------------------------------------------
void setxMinLinear(int min)
{
   xminLinear=min;
}

//---------------------------------------------------------------------------
int getxMinLinear()
{
   return xminLinear;
}

//---------------------------------------------------------------------------
void setxMaxLinear(int max)
{
   xmaxLinear=max;
}

//---------------------------------------------------------------------------
int getxMaxLinear()
{
   return xmaxLinear;
}

//---------------------------------------------------------------------------
void setxMin(int min)
{
   xmin=min;
}

//---------------------------------------------------------------------------
void setxMax(int max)
{
   xmax=max;
}

//---------------------------------------------------------------------------
void setyMin(int min)
{
   ymin1=min;
}

//---------------------------------------------------------------------------
void setyMax(int max)
{
   ymax1=max;
}

//---------------------------------------------------------------------------
void setGridFlg(bool flg)
{
   m_bGrid=flg;
}

//---------------------------------------------------------------------------
bool getGridFlg()
{
   return m_bGrid;
}

//---------------------------------------------------------------------------
void setxgrad(bool flg)
{
   xgrad=flg;
}

//---------------------------------------------------------------------------
bool getxgrad()
{
   return xgrad;
}

//---------------------------------------------------------------------------
void initialisePizPos()
{
   counter1=left1;
   counter2=left2;
}

//---------------------------------------------------------------------------
float getXValFromPixPos(int npixels)
{

   int X;
   int XVal = 0;;

   if(image == 1)
   {
      X = right1 - left1;


      if(xmin==0)
      {
         XVal = ((npixels-xmargin)*xmax )/X;
         return XVal;
      }
      else if(xmax==0)
      {
        XVal = xmin - ((npixels-xmargin)*xmin )/X;
        return XVal;
      }
      else
      {
         float D = fabs(xmin) + fabs(xmax);
         float R = fabs(xmin)/D ;
         XVal = ((npixels-xmargin)*D)/X;
         if(XVal <= R*D)
         {
            XVal = xmin + XVal;
         }
         else
         {
            XVal = XVal - R*D;
         }
         return XVal;
      }
      

   }//if(image == 1)

   return XVal;
}//getXValueFromPixPos

