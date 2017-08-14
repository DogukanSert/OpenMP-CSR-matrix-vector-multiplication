#include "read.h"

void read_matrix(int **r_ptr, int** c_ind,float** v, char*fname,int* r_count,int* v_count){	
	FILE * file;
    	if ((file = fopen(fname, "r+")) == NULL)
	{
	    printf("ERROR: file open failed\n");
	    return;
	}
	
	int column_count,row_count,values_count;
	fscanf(file, "%d %d %d\n",&row_count,&column_count,&values_count);
	*r_count = row_count;
	*v_count = values_count;
	int i;
	int *row_ptr = malloc(row_count * sizeof(int));
	int *col_ind = malloc(values_count * sizeof(int));
	for(i=0; i<values_count; i++){
		col_ind[i] = -1;
	}
	float *values = malloc(values_count * sizeof(float));
	int row,column;
	float value;
	while (1) {
		int ret = fscanf(file, "%d %d %f\n",&row,&column,&value);
		column --;
		row --;
		if(ret == 3){
			row_ptr[row]++;
		} else if(ret == EOF) {
		   	break;
		} else {
		    	printf("No match.\n");
		}
	}
    	rewind(file);
    	int index = 0;
    	int val = 0;
	for(i = 0; i<row_count;i++){
		val = row_ptr[i];
		row_ptr[i] = index;
		index += val;
	}
	fscanf(file, "%d %d %d\n",&row_count,&column_count,&values_count);
	i = 0;
	while (1) {
		int ret = fscanf(file, "%d %d %f\n",&row,&column,&value);
		column --;
		row --;
		if(ret == 3){
			while(col_ind[i+row_ptr[row]] != -1){ i++;}
			col_ind[i+row_ptr[row]] = column;
			values[i+row_ptr[row]] = value;
			i=0;
		} else if(ret == EOF) {
		   	break;
		} else {
		    	printf("No match.\n");
		}
	}
    	fclose(file);
    	*r_ptr = row_ptr;
    	*c_ind = col_ind;
    	*v = values;
}
