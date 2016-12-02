#include <stdio.h>
#include <fann.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	const unsigned int num_input = 48;
	const unsigned int num_output = 1;
	const unsigned int num_layers = 3;
	const float desired_error = (const float) 0.002;
	const unsigned int max_epochs = 200000;
	const unsigned int epochs_between_reports = 1000;
	char rna_name[30];

	if (argc != 3)
	{
		printf("Maybe you should try: %s file_name.data number_of_neurons\n", argv[0]);
		exit(0);
	}

	printf("\n%d neurons\n", (unsigned int)atoi(argv[2]));
	
	struct fann* neuronal_network = fann_create_standard(num_layers, num_input, (unsigned int)atoi(argv[2]), num_output);

	fann_set_activation_function_hidden(neuronal_network, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(neuronal_network, FANN_SIGMOID_SYMMETRIC);

	fann_train_on_file(neuronal_network, argv[1], max_epochs, epochs_between_reports, desired_error);

	sprintf(rna_name, "%d_neurons.net", atoi(argv[2]));
	fann_save(neuronal_network, rna_name);

	fann_destroy(neuronal_network);

	return 0;
}
