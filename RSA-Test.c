/*
 ============================================================================
 Name        : RSA-Test.c
 Author      : Albert Soliz Team 19
 Version     :
 Copyright   : Test the RSA implementation.
 Description : RSA (Block Cipher)Algorithm Test in C.
 ============================================================================
 */
#define CLOCKID CLOCK_REALTIME
#define SIG SIGALRM

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>


#include "mini-gmp.c"




int time_diff(struct timeval * tp1, struct timeval * tp2) {
  long sec = tp2->tv_sec - tp1->tv_sec;
  long musec = tp2->tv_usec - tp1->tv_usec;
  if (musec < 0) {
    musec += (int)1e6;
    sec--;
  }
	printf("\nSeconds:%ld \n",sec);
	printf(" microsec:%ld \n",musec);
	return sec;
}

//generate 1024 digit prime number (p,q)
//p != q
int gen_prime(mpz_t start_num,mpz_t e){
	int base = 16;
	mpz_t num_minus1;
	mpz_init(num_minus1);
	mpz_t num_test;
	mpz_init(num_test);
	unsigned long int oper_1 = 1;
	int iter = 0;

	int prime_error = mpz_probab_prime_p(start_num ,50);//test if prime
;
	int not_valid = 1;

	while(prime_error <= 0 && not_valid){
		prime_error = mpz_probab_prime_p(start_num ,50);//test if prime
		//reject if gcd(e,start_num-1) =1
		mpz_sub_ui(num_minus1,start_num,oper_1);//start_num-1
		mpz_gcd(num_test,e,num_minus1);
		int y = mpz_cmp_ui(num_test, oper_1);
		if(y != 0){
			not_valid = 0;//pass test leave whileloop;
		}
		if(prime_error == 0){
			//printf("Testing new number iteration:%d",iter);
			mpz_add_ui(start_num, start_num, oper_1);//increment test_prime_p by 1
			iter = iter+1;
		}

	}
	//printf("Testing new number iteration:%d",iter);
	//printf("\n\n");
	mpz_clear(num_minus1);
	mpz_clear(num_test);

	if (prime_error == 2){
		printf("\n\nDone with gen_prime,def\n");
		size_t print_size = mpz_out_str(stdout ,base ,start_num);//output arg1 to stdout return 0 if error

		//printf("\nDone with gen_prime,def\nsize:%d",print_size);
		if(print_size != 0)
			return print_size;
	}
	if(prime_error == 1){
		printf("\n\nDone with gen_prime,probs\n");
		size_t  print_size = mpz_out_str(stdout ,base ,start_num);//output arg1 to stdout return 0 if error

		//printf("\nDone with gen_prime,probs\nsize:%d",print_size);
		if(print_size != 0)
			return print_size;

	}
	if(prime_error == 0){
		printf("\n\nDone with gen_prime,not\n");
		size_t  print_size = mpz_out_str(stdout ,base ,start_num);//output arg1 to stdout return 0 if error

		//printf("\nDone with gen_prime,not\nsize:%d",print_size);
		if(print_size != 0)
			return print_size;

	}
	printf("\n\nDone with gen_prime,ERROR\n");

	return -1;
}

/* Tranform string from char array to decimal string
 * */
int string_to_decimal(char * in, mpz_t out){

		char text_message[] = "Hello";
		printf("Input:%s\n",text_message);

		//should be decimal 72,101,108,108,111
		//(+100) to ensure each ascii is atleast 3digits long.
		//172,201,208,208,211;

		char * c = text_message;
		char message_fixed[48];//TODO assumes only 16chars are ever GIVEN; Error check
		message_fixed[0] = '\0';
		printf  ("message_fixed:%s\n", message_fixed);

		//3digits per char

		char temp[4];
		temp[3] = '\0';
		while(*c != '\0'){
			printf("%c , ",*c);
			printf(" decimal:%d \n",((int)*c)+100);

			int num = 0;
			num = ((int)*c);
			num = num + 100;//num holds 3digit num
			printf("%d",num);
			printf  ("message_fixed before:%s\n", message_fixed);
//TODO figure way to remove sprintf, uneccessary library for 1 function
			sprintf(temp, "%d", num);//copy num(int) into temp(char array)

			//temp should be ###

			printf  ("temp is now: %s\n",temp);
			strcat(message_fixed,temp);
			printf  ("message_fixed is now:%s\n", message_fixed);

			c++;
		}
		printf("Output:%s",message_fixed);

		//172,201,208,208,211;

	return 0;
}

int decimal_to_string(mpz_t in, char * out){
	char decimal_message[] = "172201208208211";
	//
	printf("Input:%s\n",decimal_message);

	char * c = decimal_message;
	char message_fixed[48];//TODO assumes only 16chars are ever GIVEN; Error check
	message_fixed[0] = '\0';
	printf  ("message_fixed before:%s\n", message_fixed);


	//3digits per char
			char temp[4]; //hold 3 digit
			temp[3] = '\0';
			int letters = 0;
			int num_digits = 0;
			//count number of letters = len / 3;
			while(*c != '\0'){
				num_digits++;
				c++;
			}

			letters = num_digits/3;
		printf("Num letters:%d", letters );
		int a = 0;
		c = decimal_message;
		int parsed_num = 0;
		while(*c != '\0'){
				printf("%c , ",*c);
				int dig = 0;
				printf(" decimal:%c \n",*c);
				dig = (int)*c;
				printf  ("message_fixed:%s\n", message_fixed);
				//get 3 dig into char temp
				temp[a] = dig;
				a++;

				if(a > 2 ){
					a = 0;
					//sprintf(temp, "%d", dig);//copy dig into temp
					printf("temp is now: %s\n", temp);
					//turn temp from digit to ascii by -100 then cast char


					parsed_num = atoi(temp);
					//fix the initial padding form str-dec conversion
					parsed_num = parsed_num - 100;

					printf("parsed_num is now: %d\n", parsed_num);
					//ascii?
					char parsed_letter[2];
					parsed_letter[0] = (char)parsed_num;
					parsed_letter[1] = '\0';
					printf("parsed_letter is now: %c\n", parsed_letter[0]);

					strcat(message_fixed,parsed_letter);
					printf("message_fixed is now:%s\n", message_fixed);
				}

			c++;
			//reset temp;
		}
		printf("Output:%s",message_fixed);

		//"Hello"

	return 0;
}

int encrypt_message(mpz_t message, mpz_t e, mpz_t n,mpz_t message_enc){
	//m^e * mod n = c
	//c is message_enc

	mpz_powm(message_enc, message, e, n);//Set message_enc to message ^ e * mod n

	return 0;
}

int decrypt_message(mpz_t message_enc, mpz_t d, mpz_t n, mpz_t message_dec){
	//c^d * mod n = m
	//m is message_dec

	mpz_powm(message_dec, message_enc, d, n);//Set message_dec to message_enc ^ d * mod n

	return 0;
}


//generate totient_n = (p-1)*(q-1);
int generate_totient_n(mpz_t p, mpz_t q, mpz_t totient_n){
	//input : p,q
	//output totient_n
		mpz_t p_minus1;
		mpz_init(p_minus1);
		mpz_t q_minus1;
		mpz_init(q_minus1);
		unsigned long int oper_1 = 1;
		mpz_sub_ui(p_minus1, p, oper_1);//set p_minus1 = p - 1;
		mpz_sub_ui(q_minus1, q, oper_1);//set q_minus1 = q - 1;

		mpz_mul(totient_n, p_minus1, q_minus1); //set totient_n = p-1*q-1;
		mpz_clear(p_minus1);
		mpz_clear(q_minus1);
		printf("\n\n");

		size_t print_size4 = mpz_out_str(stdout ,16 ,totient_n);//output totient_n to stdout return 0 if error
		printf("\nDone with totient_n: size: %ld\n",print_size4);

	return 0;
}

//private key, d
int generate_d(mpz_t e, mpz_t totient_n , mpz_t d){
	//inverse of e with totient_n //extend euclid algor
	//void mpz_gcdext (mpz_t, mpz_t, mpz_t, const mpz_t, const mpz_t);
	//mpz_gcdext(d , , , e , totient_n );
	//void mpz_gcd (mpz_t, const mpz_t, const mpz_t);
	//mpz_gcd(d,e,totient_n);
	//d = 1mod(totient_n) / e;

	mpz_invert(d , e,totient_n);

	printf("\n\n");
	size_t print_size5 = mpz_out_str(stdout ,16 ,d);//output temp to stdout return 0 if error
	printf("\nDone with d: size: %ld\n",print_size5);

	return 0;
}


int send_output(mpz_t message){
	int size = mpz_sizeinbase(message,10);
	size = size + 2;
	char t[size];
	char * a = mpz_get_str(t,10,message);
	printf("%s\n",a);

	FILE* fd;
	//char* filename = "/home/bitnami/progs/output.txt";
	char* filename1 = "./output.txt";
	int errno;

	fd = fopen(filename1,"w+");
	if(fd != NULL){
		printf("Writing outputfile...\n");
		fputs(a,fd);
		fclose(fd);
	}
	else {
		printf("\nfd failed:%s\n",strerror(errno) );
		return -1;
	}
	return 0;
}



int RSA(mpz_t p, mpz_t q) {
	printf("\nRSA.c\n\n"); /* prints RSA.c*/

//	mpz_t p; //p
	//mpz_t q; //q
	//p != q
	mpz_t n; //n = p * q
	mpz_t totient_n;
	mpz_init(totient_n);
	mpz_t e; //e
	mpz_init_set_str(e, "65537",10);
	mpz_t d; //d = gcd (e,totient_n)
	mpz_init(d);
	mpz_init(n);
	mpz_t message_enc;
	mpz_init(message_enc);
	mpz_t message;
	//message is "attack at dawn"
	mpz_init_set_str(message, "1976620216402300889624482718775150",10);

	//mpz_init_set_str(p, "35028512345478748225956220793734512128733387803682075433653899983955179850988797899869146900809131611153346817050812027524255478748885956220793734512128733387803682075433653899983955179850988797899869146900320960221601463663463918124709871054152331202752425547874888595622079373451212873338780368207543365389998395517985098879789986914690080913161115334681705083209602216014636634639181247098710541523312027524255478748885956220793734512165433387803682075433653899983955173550988797899869146900809131611153346811",10);
	//mpz_init_set_str(q, "73027524252478748885956220793734512128733387803682075433653899983955179850988797899869146900809131611153346817050812027524255478748885956220793734512128733387803682075433653899983955179850988797899869146900320960221601463663463918124709871054152331202752425547874888595622079373451212873338780368207543365389998395517985098879789986914690080913161115334681705083209602216014636634639181247098710541523312027524255478748885956220793734512128733387803682075433659855983955179850988797899869146900809131611153326852",10);

	//Hardcode to test if it works
	//mpz_init_set_str(p, "12131072439211271897323671531612440428472427633701410925634549312301964373042085619324197365322416866541017057361365214171711713797974299334871062829803540",10);
	//mpz_init_set_str(q, "12027524255478748885956220793734512128733387803682075433653899983955179850988797899869146900809131611153346817050832096022160146366346391812470987105415232",10);

	//start point

	int p_size = gen_prime(p,e);
	printf("\n\n");

	int q_size = gen_prime(q,e);
	int comp = mpz_cmp(p, q);
	if(comp != 0){
		printf("\n\n");
		mpz_out_str(stdout ,16 ,p);//output arg1 to stdout return 0 if error
		printf("\nDone with p: size: %d\n",p_size);
		printf("\n\n");

		mpz_out_str(stdout ,16 ,q);//output arg2 to stdout return 0 if error
		printf("\nDone with q: size: %d\n",q_size);
		printf("\n\n");
		//generate n = p * q
		mpz_mul(n, p, q);
		size_t  print_size3 = mpz_out_str(stdout ,16 ,n);//output arg3 to stdout return 0 if error
		printf("\nDone with n: size: %ld\n",print_size3);


		generate_totient_n(p,q, totient_n);

		//public key
//https://www.bigprimes.net/archive/prime/102/
		//set e = 65537; strong prime
//generate d priv key
		generate_d(e , totient_n , d);

		printf("\nMessage:\n");
		mpz_out_str(stdout ,16 ,message);//output message to stdout return 0 if error
		printf("\n\n\n\nPRINTIN GNEW SRRINGS \n\n");
		int size = mpz_sizeinbase(message,10);
		size = size + 2;
		char t[size];
		char * a = mpz_get_str(t,16, message);
		printf(":%s\n",a);
		printf("\n\n\n\nDONE PRINTIN GNEW SRRINGS \n\n");
		//string_to_decimal(str ,message);
int pass = 0;
signed long int cnt = 0;
signed long int y = 0;
time_t tim;
srand( (unsigned) time(&tim) );
signed long int random1 = rand();
signed long int random2 = rand();

//printf("\nUsing rand value: %lu \n", random );
struct timeval enc_start, enc_end; /* Used to compute elapsed time. */
struct timeval dec_start, dec_end; /* Used to compute elapsed time. */

for(y = 0; y < 10; y++){//TODO too long to run
		//set message to y;
		random1 = rand();
		random2 = rand();//up to 32,000 at least
		mpz_t val;mpz_init(val);
		mpz_t ra1;mpz_init_set_si(ra1,random1);mpz_t ra2;mpz_init_set_si(ra2,random2);
// val = ra1 ^ 8 * ra2 ^ 8
		mpz_addmul(val,ra1 ,ra2 );//val = ra1 * ra2;30k*30k
			mpz_addmul(val,ra1 ,ra2 );//val = ra1 * ra2;
			mpz_addmul(val,ra1 ,ra2 );//val = ra1 * ra2;
			mpz_addmul(val,ra1 ,ra2 );//val = ra1 * ra2;
			mpz_addmul(val,ra1 ,ra2 );//val = ra1 * ra2;12digits
			mpz_addmul(val,ra1 ,ra2 );//val = ra1 * ra2;
			mpz_addmul(val,ra1 ,ra2 );//val = ra1 * ra2;20digits
			mpz_addmul(val,ra1 ,ra2 );//val = ra1 * ra2;
		
		mpz_set(message ,val);//set msg to random1 * random2;
		gettimeofday(&enc_start, 0); // start timer
	encrypt_message(message, e, n, message_enc);
		gettimeofday(&enc_end, 0);

/*
		printf("\nMessage_enc:\n");
		mpz_out_str(stdout ,10 ,message_enc);//output message to stdout return 0 if error
		printf("\n\n\n\nPRINTIN GNEW SRRINGS2 \n\n");
		int size1 = mpz_sizeinbase(message_enc,10);
		size1 = size1 + 2;
		char x[size1];
		char * b = mpz_get_str(x,10, message_enc);
		printf(":%s\n",b);
		printf("\n\n\n\nDONE PRINTIN GNEW SRRINGS2 \n\n");
*/
		//make copy of message
		mpz_t message_copy;
		mpz_init(message_copy);
		mpz_add(message_copy,message_copy,message);//0-> 0+x

		gettimeofday(&dec_start, 0); // start timer
	decrypt_message(message_enc, d, n, message);
		gettimeofday(&dec_end, 0);

		//test if message_copy == message
		pass = mpz_cmp(message_copy,message);
		if(pass == 0){
			printf("Time to encrypt: ");
			time_diff(&enc_start, &enc_end);
			printf("Time to decrypt: ");
			time_diff(&dec_start, &dec_end);

			//send_output(message_enc);

			cnt++;
		}
		mpz_clear(message_copy);
		mpz_clear(val);
}

printf("Passed %ld/%ld\n",cnt,y);
mpz_init_set_str(message, "1976620216402300889624482718775150",10);
encrypt_message(message, e, n, message_enc);
mpz_out_str(stdout ,10 ,message_enc);//output arg2 to stdout return 0 if error
send_output(message_enc);
decrypt_message(message_enc, d, n, message);

		//decimal_to_string(message, str);

	}

	mpz_clear(message_enc);
	
	mpz_clear(e);
	mpz_clear(d);
	mpz_clear(n);
	mpz_clear(totient_n);
	mpz_clear(message);

	if(comp == 0 ){
		printf("ERROR: p == q");
		return -1;
	}


	return 0;
}

int main(int argc, char * argv[]) {

	mpz_t test_p;
	mpz_t test_q;
	mpz_init_set_str(test_p,"e021757c777288dacfe67cb2e59dc02c70a8cebf56262336592c18dcf466e0a4ed405318ac406bd79eca29183901a557db556dd06f7c6bea175dcb8460b6b1bc05832b01eedf86463238b7cb6643deef66bc4f57bf8ff7ec7c4b8a8af14f478980aabedd42afa530ca47849f0151b7736aa4cd2ff37f322a9034de791ebe3f51",16);
	mpz_init_set_str(test_q,"ed1571a9e0cd4a42541284a9f98b54a6af67d399d55ef888b9fe9ef76a61e892c0bfbb87544e7b24a60535a65de422830252b45d2033819ca32b1a9c4413fa721f4a24ebb5510ddc9fd6f4c09dfc29cb9594650620ff551a62d53edc2f8ebf10beb86f483d463774e5801f3bb01c4d452acb86ecfade1c7df601cab68b065275",16);
	mpz_t test_e; mpz_init_set_str(test_e,"65537",10);

	int opt = 0;
	while ((opt = getopt(argc, argv, "p:q:")) != -1) {
		switch (opt) {
			case 'p':
				mpz_init_set_str(test_p,optarg,16);
				mpz_out_str(stdout ,16 ,test_p);//output
				printf("\n");
				break;
	        case 'q':
				mpz_init_set_str(test_q,optarg,16);
				mpz_out_str(stdout ,16 ,test_q);//output
	            break;
		}
	}
//

	//RSA();



	//Test if P & Q are actually prime

	int prime_error_p = mpz_probab_prime_p(test_p,50);//test if prime
	int prime_error_q = mpz_probab_prime_p(test_q,50);//test if prime

	//0 = not prime; 1 = probably prime; 2 = definatley prime

	if(prime_error_p > 0 && prime_error_q > 0){
		printf("\nTEST result: POSITIVE\nRunning subsequent tests\n");
		RSA(test_p,test_q);
	}
	else{
		printf("\nTEST result: NEGATIVE\nTesting parameter generation\n");
		struct timeval para_start, para_end; /* Used to compute elapsed time. */
		
		gettimeofday(&para_start, 0);
		gen_prime(test_p,test_e);//parameters
		gen_prime(test_q,test_e);//parameters
		gettimeofday(&para_end, 0);
		printf("Time to generate parameters: ");
		time_diff(&para_start, &para_end);

	}
	mpz_clear(test_p);
	mpz_clear(test_q);
	mpz_clear(test_e);

	//Test if P != Q



	printf("Done");
	return 0;
}
