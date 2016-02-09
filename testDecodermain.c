#include "decoder.c"

main ( int argc, char **argv ) {
     
		printf("Chaine1 : %s \n", generateMulticast("poney"));
		printf("Chaine2 : %s \n", generateLogin("Pierre","provisoire"));
		printf("Chaine3 : %s \n", generateLeave("Pierre"));
		printf("Chaine4 : %s \n", generateMsg("Pierre","poney"));
		printf("Chaine5 : %s \n", generateWhisp("Pierre","Thomas","poney"));

		message_parsed_t *m1 = malloc(sizeof(message_parsed_t));
		m1 = decode("0;poney");



     return 0;
}
