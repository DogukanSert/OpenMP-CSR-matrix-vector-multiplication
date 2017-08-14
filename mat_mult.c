#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "read.h"
#include <float.h>
int main (int argc, char* argv[]){
	if ( argc != 5){
		printf( "Incorrect usage");
	}
	else{
		int* row_ptr;
		int* col_ind;
		float* values;
		int r_count, v_count, i, k,r;
		int thread_num = atoi(argv[1]);
		int repetitions = atoi(argv[2]);
		int mode = atoi(argv[3]);
		char* fname = argv[4];
		read_matrix(&row_ptr, &col_ind, &values, fname, &r_count, &v_count);
		float* x = malloc(r_count* sizeof(float));
		for(i = 0; i<r_count;i++){
		    	x[i]= 1.0;
		}
		double start, end;
		omp_set_num_threads(thread_num);
		float temp;
		if(mode == 1){
			fprintf(stdout,"Initial Matrix\n");
			for(i = 0; i<r_count;i++){
		    		if(i+1 < r_count){
		    			for(k = row_ptr[i]; k < row_ptr[i+1];k++){
		    				fprintf(stdout,"%d %d %.10f\n",i+1,col_ind[k]+1,values[k]);
		    			}
		    		}else{
		    			for(k = row_ptr[i]; k< v_count; k++){
		    				fprintf(stdout,"%d %d %.10f\n",i+1,col_ind[k]+1,values[k]);
		    			}
		    		}
		    	}
		    	fprintf(stdout,"Initial Vector\n");
		    	for(i = 0; i<r_count;i++){
		    		fprintf(stdout,"%f\n",x[i]);
		    	}
	  	} 
	  	float y[r_count];
	  	start = omp_get_wtime();
		for(r = 0; r<repetitions; r ++){
			#pragma omp parallel for private(temp,k,i)
	    		for(i = 0; i<r_count;i++){
	    			temp = 0.0;
		    		if(i+1 < r_count){
		    			for(k = row_ptr[i]; k < row_ptr[i+1];k++){
		    				#pragma omp atomic	
		    				temp += (float)(values[k]*x[col_ind[k]]);
		    			}
		    		}else{
		    			for(k = row_ptr[i]; k< v_count; k++){
		    				#pragma omp atomic	
		    				temp += (float)(values[k]*x[col_ind[k]]);
		    			}
		    		}
		    		y[i] = temp;
		    	}
		    	#pragma omp barrier
		    	for(i = 0; i<r_count; i++){
		    		x[i] = y[i];
		    	}
		}
		end = omp_get_wtime() - start;
		if(mode == 1){
			fprintf(stdout,"Resulting Vector\n");
		    	for(i = 0; i<r_count;i++){
		    		fprintf(stdout,"%f\n",x[i]);
		    	}
		}
		printf("%lf\n",end);		
	}
}
