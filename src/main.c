#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gsl/gsl_fit.h>
 
#define MAX_LINE_SIZE 25
 
int main(int argc, const char * argv[]) {
    
    char file_name[] = "./data/spring_data.csv";
    FILE *fp;
    fp = fopen(file_name, "r");
    
    if (!fp) {
        fprintf(stderr, "failed to open file for reading\n");
        return 1;
    }
    
    char line[MAX_LINE_SIZE];
    char *result = NULL;
    double Y[19];
    double X[19];
    int x = 0;
    int y = 0;
        
    while(fgets(line, MAX_LINE_SIZE, fp) != NULL) {
        
    result = strtok(line, ",");
    int i = 0;
    while( result != NULL ) {
    if(i!=0){
      if(atof(result)>0){
        X[x] = atof(result);
        x++;
        }
    }
    else{
      if(atof(result)>0){
        Y[y] = atof(result);
        y++;
        }
      i++;
    }
    result = strtok(NULL, ",");
    }
  }
    fclose (fp);
    for (int n=0;n<19;n++)
      {
        X[n]= X[n]*9.81;
      }

    double c0, c1, cov00, cov01, cov11, sumsq;
    gsl_fit_linear (X, 1, Y, 1, 19,
                   &c0, &c1, &cov00, &cov01, &cov11,
                   &sumsq);
    printf ("# best fit: Y = %g + %g X\n", c0, c1);
    printf ("covariance matrix:\n");
    printf ("[ %g, %g\n  %g, %g]\n", cov00, cov01, cov01, cov11);
    printf ("sumsq = %g\n", sumsq);
    printf ("\n");


    FILE *pipe = popen("gnuplot -persist", "w");  
    int n = 19;
    if (pipe) {  
      fprintf(pipe, "set term wx\n");                 
      fprintf(pipe, "set xlabel '|Force| (Newtons)'\n");
      fprintf(pipe, "set ylabel 'Distance (meters)'\n");
      fprintf(pipe, "set title '<X,Y> and Linear fit:y=%.4f*x+%.4f'\n",c1,c0);

    
     // 1 sending gnuplot the plot '-' command
     fprintf(pipe, "plot '-' title '<x,y>' with points  pt 7 lc rgb 'blue',\
                         '-' title 'Line' with  linespoints  pt  6 lc rgb 'red'\n");
     
     // 2 followed by data points: <x,y>
     for (int i = 0; i < n; i++)
     {
        fprintf(pipe, "%lf %lf\n", X[i], Y[i]);
     }
     // 3 followed by the letter "e" 
     fprintf(pipe, "e");
     
     // linear fit
     fprintf(pipe,"\n"); // start a new draw item
     fprintf(pipe, "%lf %lf\n", 0.0, c0+c1*0);
     for (int i = 0; i < n; i++)
     {
        fprintf(pipe, "%lf %lf\n", X[i], c0+c1*X[i]);
     }
     fprintf(pipe, "%lf %lf\n", 10.0,c0+c1*10);
     fprintf(pipe, "e");
      
     fflush(pipe);
     fprintf(pipe, "exit \n"); // exit gnuplot
     pclose(pipe);             //close pipe
    }
    return 0;
}
