#include <stdio.h>
#include <fann.h>
#include <math.h>
#include <stdlib.h>

typedef unsigned int uint;

#define MESS_DATA 1

double error(fann_type *expected, fann_type *input, uint number_inputs);

int main(int argc, char *argv[])
{
	time_t t;
	srand((unsigned) time(&t));

	struct fann* neuralNetwork;
	fann_type **data;
	fann_type *answer;
	fann_type *read_answer;
	FILE *f;
	uint i, j;
	uint number_inputs = 0;
	const uint number_data = 48;

	if (argc != 4)
	{
		printf("Maybe you should try: %s file_name.net file_name.data number_inputs\n", argv[0]);
		exit(0);
	}

	neuralNetwork = fann_create_from_file(argv[1]);
	if(!neuralNetwork)
	{
		printf("Neural network %s unavailable! - ABORTADO\n", argv[1]);
		exit(0);
	}

	f = fopen(argv[2], "r");
	if(f == NULL)
	{
		printf("File %s unavailable! - Aborting\n", argv[2]);
		exit(0);
	}

	number_inputs = (unsigned int)atoi(argv[3]);
	if(number_inputs < 1)
	{
		printf("Number of inputs = %s is incorrect! - Aborting\n", argv[3]);
		exit(0);		
	}

	//Creating matrix for the input data
	data = (fann_type**)calloc(number_inputs, sizeof(fann_type*));
	for(i = 0; i < number_inputs + 1; i++)
		data[i] = (fann_type*)calloc(number_data, sizeof(fann_type));
	//Creating vector for the answer
	read_answer = (fann_type*)calloc(number_inputs, sizeof(fann_type));
	//Reading file and saving on the vector and on the matrix
	for (i = 0; i < number_inputs; i++)
	{
		for(j = 0; j < number_data; j++)
		{
			fscanf(f, "%f ", &data[i][j]);
#ifdef MESS_DATA			
			data[i][j] = data[i][j] + (float)(rand() % 500)/1000.0;	
#endif				
		}

		fscanf(f, "%f ", &read_answer[i]);

	}
	fclose(f);

	answer = (fann_type*)calloc(number_inputs, sizeof(fann_type));

	for(i = 0; i < number_inputs; i++)
	{
		fann_reset_MSE(neuralNetwork);
		answer[i] = *(fann_run(neuralNetwork, data[i]));
		printf("%f\n", answer[i]);
	}

	printf("Mean Squared Error  = %lf\n", error(answer, read_answer, number_inputs));

	fann_destroy(neuralNetwork);		
	return 0;
}

double error(fann_type *expected, fann_type *input, uint number_inputs)
{
	double erro = 0;
	uint i;
	for(i = 0; i < number_inputs; i++)
			erro += pow(expected[i] - input[i], 2) / number_inputs;

	return pow(erro, 0.5);	
}
