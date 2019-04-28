/*
 ============================================================================
 Name        : ElGamal-Test.c
 Author      : Albert Soliz Team 19
 Version     :
 Copyright   : Uses mini-gmp as the bignum library. Open-source.
 Description : ElGamal Algorithm in C.
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

//try to implement from gmp.h into mini-gmp.h to cut size of file
//Based on Lecture 15: Elgamal Encryption Scheme by Christof Paar



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


int send_output(mpz_t message1,mpz_t message2){
	int size1 = mpz_sizeinbase(message1,10);
	size1 = size1 + 2;
	char t[size1];
	char * a = mpz_get_str(t,10,message1);
	printf("%s\n",a);
	int size2 = mpz_sizeinbase(message2,10);
	size2 = size2 + 2;
	char t2[size2];
	char * b = mpz_get_str(t2,10,message2);
	printf("%s\n",b);


	FILE* fd;
	//char* filename = "/home/bitnami/progs/output.txt";
	char* filename1 = "./output.txt";
	char* filename2 = "C:/Eclipse/Workspace/ELGAMAL-Test/src/output_ELGAMAL.txt";
	int errno;

	fd = fopen(filename1,"w+");
	if(fd != NULL){
		printf("Writing outputfile...\n");
		fputs("c1=",fd);
		fputs(a,fd);
		fputs("\nc2=",fd);
		fputs(b,fd);
		fclose(fd);
	}
	else {
		printf("\nfd failed:%s\n",strerror(errno) );
		return -1;
	}
	return 0;
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
		//mpz_init_set_str(message,"172201208208211",10);
		mpz_init_set_str(out,message_fixed,10);
		int base = 10;
		printf("\n");
		size_t  print_size0 = mpz_out_str(stdout ,base , out);//output p to stdout return 0 if error
		printf("\nDone with out,\nsize:%ld",print_size0);
		printf("\n\n");


		printf("Output:%s",message_fixed);

		//172,201,208,208,211;

	return 0;
}

int decimal_to_string(mpz_t in, char * out){
	char decimal_message[] = "172201208208211";
	//
	printf("\nInput:%s\n",decimal_message);

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

		//mpz_init_set_str(message,"172201208208211",10);
		/*mpz_init_set_str(in,message_fixed,10);
		int base = 10;
		printf("\n");
		size_t  print_size0 = mpz_out_str(stdout ,base , in);//output p to stdout return 0 if error
		printf("\nDone with in,\nsize:%d",print_size0);
		printf("\n\n");
*/
		printf("Output:%s",message_fixed);

		//"Hello"

	return 0;
}

int encrypt_message(mpz_t message, mpz_t p, mpz_t q, mpz_t g ,mpz_t X,mpz_t c1,mpz_t c2){
	//from https://www.di-mgt.com.au/public-key-crypto-discrete-logs-3-elgamal.html
	//input p,q,g,  recipeint public key X, message m (0,p-1)
	//ouput ciphertext (c1,c2)
	printf("\n\nEncrypting message...\n");
	int base = 16;
	printf("\n");
	size_t  print_size10 = mpz_out_str(stdout ,base , message);//output p to stdout return 0 if error
	printf("\nusing message:,\nsize:%ld",print_size10);
	printf("\n\n");

	//choose k from 1 -> p-1;
	mpz_t k;
	mpz_t temp1;
	mpz_t temp2;
	mpz_t temp3;
	mpz_init(temp1);
	mpz_init(temp2);
	mpz_init(temp3);

	mpz_init(k);
	mpz_t exp;
	mpz_init(exp);

	//set k = random for every message?;
	mpz_init_set_str(k,"145654165165",10);
	//c1 = g^k mod p
	mpz_powm(c1,g,k,p);//c1
	//m = message
	//B = already set
	//temp1 = m mod p
	mpz_mod(temp1,message,p);
	//Compute c2 = m * X^k mod p
	//temp2 = powm X ^k mod p
	mpz_powm(temp2, X,k,p);

	//temp3 = (temp1 * temp2 ) mod p
//(A*B) mod n = (a mod n * b mod n) mod n
	mpz_mul(temp3,temp1,temp2);
	mpz_mod(c2,temp3,p);
//print
	printf("\n");
	size_t  print_size0 = mpz_out_str(stdout ,base , c1);//output p to stdout return 0 if error
	printf("\nDone encrypting with c1,\nsize:%ld",print_size0);
	printf("\n\n");
	printf("\n");
	size_t  print_size1 = mpz_out_str(stdout ,base , c2);//output p to stdout return 0 if error
	printf("\nDone encrypting with c2,\nsize:%ld",print_size1);
	printf("\n\n");



	mpz_clear(temp1);
	mpz_clear(temp2);
	mpz_clear(temp3);
	mpz_clear(exp);
	mpz_clear(k);
	//return c1,c2
	return 0;

}

//https://www.di-mgt.com.au/public-key-crypto-discrete-logs-3-elgamal.html
int decrypt_message(mpz_t c1,mpz_t c2, mpz_t p, mpz_t q, mpz_t g, mpz_t x, mpz_t message){
	//input p,q,g,  recipeints private key x, ciperhetext (c1,c2)
	//output m = message
	printf("\n\nDecrytpting messsagee..\n");
	int base = 16;
	printf("\n");
	size_t  print_size10 = mpz_out_str(stdout ,base , c1);//output p to stdout return 0 if error
	printf("\nusing c1:,\nsize:%ld",print_size10);
	printf("\n\n");
	size_t  print_size11 = mpz_out_str(stdout ,base , c2);//output p to stdout return 0 if error
	printf("\nusing c2:,\nsize:%ld",print_size11);
	printf("\n\n");

	mpz_t temp1;
	mpz_t temp2;
	mpz_t temp3;
	mpz_init(temp1);
	mpz_init(temp2);
	mpz_init(temp3);
	mpz_t exp;
	mpz_init(exp);
//Compute m = c1^(p−x−1) c2 mod p
//aka (c1^p-x-1 mod p * c2 mod p ) mod p
	//set exp = p - x - 1;
	mpz_sub(exp,p,x);//p - x
	unsigned long int oper_1 = 1;
	mpz_sub_ui(exp,exp,oper_1);//p-b-1;

	//set temp1 = c1^exp mod p;
	mpz_powm(temp1,c1,exp,p);

	//set temp2 = c2 mod p;
	mpz_mod(temp2,c2,p);

	mpz_mul(temp3,temp1,temp2);//line -10
	//m = temp3 mod p
	mpz_mod(message,temp3,p);

	printf("\n");
	size_t  print_size0 = mpz_out_str(stdout ,base , message);//output p to stdout return 0 if error
	printf("\nDone decrypting:  message_dec,\nsize:%ld",print_size0);
	printf("\n\n");



	mpz_clear(exp);
	mpz_clear(temp1);
	mpz_clear(temp2);
	mpz_clear(temp3);
	//return m
	return 0;
}


//based on theory/technique from https://www.di-mgt.com.au/multiplicative-group-mod-p.html
int generate_p_q_g(mpz_t p, mpz_t q , mpz_t g){
	//select a prime p so that p = jq + 1 where j is a large even integer
		//sometimes called the cofactor
	//set q as 256 bit prime = 78 digits //https://security.stackexchange.com/questions/37907/how-long-is-a-2048-bit-rsa-key
	//set p as prime 617 digit long

	//mpz_init_set_str(pp,"259285516004991237904610998519859774324030594532751736827621729122927392202102856447445102732032054909127076144987063481173324548641005957405057813533022233920924436586536106720985119936293052303884274172392989827541435411699989981233114965119198963954361280362382857929807678478402693362461823089998035361280362382857929807678478402693362461823089998035361280362382857929807678478402693362461823089998035361280362382857929807678478402693362461823089998035361280362382857929807678478402693362461823089998035361280362382857929807678478402693362461823089998035361280362382857929807678478402693362461823089998035361280529461736109787829377629426551503887896042799049180307475911600113536618063604052",10);
	//mpz_init_set_str(p,"19327210897467885519624495407304217845488409100133554803661172025039322784872775172789521895444178690740428588185031695453815386756662619555849446656794905221115788002016245291768283472480460523777510973085032471711187806590185987219179345022033106753600355795626394426859896564719805266547324204357196851217",10);
		//1932 test vector

	//mpz_init_set_str(q,"983633858469108611936846792207646525014934079943",10);
//98 test vector
	//mpz_init_set_str(qq,"157662021640230088962448271877515019766202164023008896244827187751506546546544",10);
	//TODO remove hardcoded test vector
	//mpz_init_set_str(g,"2008851267811649301382055697326002225321501629224616043097959307844472637339783779480891271906681929732776937543331689329117914118665148580824850572191418544875109802154341862162654424065963144063936607375606796563706389362731767772194368576684632589065496658911743756860379357301492526015846031839304359976",10);


	//print p & alpha
	unsigned long int oper_1 = 1;
	mpz_t j;
	mpz_init_set_str(j,"19648785705229832512739279235719869326994578857372093636756123297040568424679124645400612341931690579457038046947983396198878406475643907265436455907971703045747694698970488141521321179649598545010351796154042242157997633368825998252795580916414671992092563312",10);
	//19 is test vector
	  //mpz_init_set_str(j,"1644565465465465165165465465131651646548787654321326549879875316548435465494513515687546515486876546516516548488765465165184989874654654849879873213216546541332165165454651616482718775150197662021640230088962448271877515065465482718775150197662021640230088962448271877515065465482718775150197662021640230088962448271877515065465482718775150197662021640230088962448271877515065465482718775150197662021640230088962448271877515065465482718775150197662021640230088962448271877515065465482718775150197662021640230088962448271877515065465482718775150197662021640230088962448271877515065465482718775150197662021640230088962448",10);
	int not_passed = 1;
	int q_found = 0;
	mpz_t temp;
	mpz_init(temp);

	while(not_passed){
		//find q until prime
		q_found = mpz_probab_prime_p(q ,50);//test if q probably a prime
		if(q_found == 0){
			mpz_add_ui(q, q, oper_1);//increment q by 1

		}
		//test q is prime?
		if(q_found > 0){//if p is prime, q is prime factor, j is large even integer
			//if random h 1<h<p-1 & g = h^j (mod p )
			//if g > 1 done; else new h

			int p_found = 0;
			int p_continue = 1;
			//test random ints such that p = qz + 1;
			while(p_continue){
				//test if p = qj + 1 is prime
				//set p to value
					//set temp = q*j;
				mpz_mul(temp, q , j);
					//set p to temp+1
				mpz_add_ui(p, temp, oper_1);

				//test new p = qz + 1
				p_found = mpz_probab_prime_p(p ,50);//test again


				if(p_found > 0){
					p_continue = 0;//exit iwhiel loop
				}
				if(p_found == 0){
					//if not add 1 to j reloop
					mpz_add_ui(j, j , oper_1);
				}

			}//p cont

			not_passed = 0;//escape global while looop
		}


	}//not passed
	// p = qj + 1;
	//make g
	//j = p-1 / q
	//set g = h^( (p-1)/q ) mod p
	//set g = h^j mod p
	mpz_t m;
	mpz_t exp;
	mpz_init(m);
	mpz_init(exp);
	mpz_t h;
	mpz_init_set_str(h,"54165156651165",10);//TODO make actually random later
	mpz_t p_sub1;
	mpz_init(p_sub1);
	mpz_t quotient;
	mpz_t remainder;
	mpz_init(quotient);
	mpz_init(remainder);


	//https://crypto.stackexchange.com/questions/56155/primitive-root-of-a-very-big-prime-number-elgamal-ds
/*
	//m = random integer m modulo p is generated
	//1738 mod
	mpz_mod(m, h, p);//TODO remove this
	int base = 10;

	printf("\n");
	size_t  print_size10 = mpz_out_str(stdout ,base , m);//output p to stdout return 0 if error
	printf("\nDone with m,\nsize:%d",print_size10);
	printf("\n\n");
*/
	// set exp = (p−1)/q
	//exp = j
	mpz_sub_ui(p_sub1, p , oper_1);//p_sub1 = p-1
	mpz_cdiv_qr(exp,remainder ,p_sub1, q);//TODO check if div is exact or not?
	unsigned long int oper_0 = 0;
	int fail = 1;
	while(fail==1){
		//printf("FAILED TO find quotient with nonremainder\n\n");
		int comp1 = mpz_cmp_ui(remainder,oper_0);
		if(comp1 != 0){///add one to h rerun till not -1
			mpz_add_ui(h,h,oper_1);
			mpz_cdiv_qr(exp, remainder ,p_sub1, q);

		}
		if(comp1 == 0){
			fail = 0;//exit whileloop
		}
	}
//https://www.di-mgt.com.au/public-key-crypto-discrete-logs-1-diffie-hellman.html
	mpz_powm(g, h , j , p);//Set rop to base exp mod mod.
	//TODO add powm vs hardcode

	fail = 1;
	while(fail==1){
		int comp = mpz_cmp_ui(g,oper_1);//check if g == 1 //aka g > 1; g < p-1
		if(comp == 0){
			printf("\nFAILD TO FIND g != 1;\n");
			printf("\n\nNeed to test with new value m or h");

			return -1;
		}
		if(comp != 0){
			fail = 0;//exit whileloop
		}

	}


	//check that g !=1
	mpz_clear(quotient);
	mpz_clear(remainder);
	mpz_clear(exp);
	mpz_clear(h);
	mpz_clear(p_sub1);
/*
	printf("\n");
	size_t  print_size0 = mpz_out_str(stdout ,base , qq);//output p to stdout return 0 if error
	printf("\nDone with q,\nsize:%d",print_size0);
	printf("\n\n");
	size_t  print_size = mpz_out_str(stdout ,base , pp);//output p to stdout return 0 if error
	printf("\nDone with p,\nsize:%d",print_size);
	printf("\n");
	size_t  print_size1 = mpz_out_str(stdout ,base , j);//output p to stdout return 0 if error
	printf("\nDone with j,\nsize:%d",print_size1);
	printf("\n");

	printf("\n");
	size_t  print_size2 = mpz_out_str(stdout ,base , gg);//output p to stdout return 0 if error
	printf("\nDone with g,\nsize:%d",print_size2);
	printf("\n");
*/

/*
	mpz_clear(gg);
	mpz_clear(pp);
	mpz_clear(qq);

	mpz_clear(j);*/
	mpz_clear(temp);
	return 0;
}

int generate_X_x(mpz_t p, mpz_t q, mpz_t g, mpz_t x, mpz_t X){
//input p,q,g
	//output x = priv, X = publickey
	//select x 2 -> q-2
	//TODO fix this
	//compute X = g^x mod p
	mpz_powm(X,g,x,p);//
	//return x, X//keep 'x' secret

	int base = 10;
	printf("\n");
	size_t  print_size0 = mpz_out_str(stdout ,base , X);//output p to stdout return 0 if error
	printf("\nDone with X,\nsize:%ld",print_size0);
	printf("\n\n");

	return 0;
}

int generate_Z_x(mpz_t p, mpz_t q, mpz_t g, mpz_t X, mpz_t y, mpz_t Z){//computation of shared secret by party Y
	//input p,q,g,    X,y
	//Shared secret Z

	//check 1 < X < p
	int temp = mpz_cmp(X,p);
	if(temp >= 0){
		return -1;
	}
	//check X^q mod p ==1
//TODO do this ^^^
	//compute Z = X^y mod p

	mpz_powm(Z,X,y,p);
	//return Z
	int base = 10;
	printf("\n");
	size_t  print_size0 = mpz_out_str(stdout ,base , Z);//output p to stdout return 0 if error
	printf("\nDone with Z from a,\nsize:%ld",print_size0);
	printf("\n\n");

	return 0;
}

int ELGAMAL(mpz_t p , mpz_t q,mpz_t g,  mpz_t j ) {
	printf("ElGamal.c\n\n"); /* prints ElGamal.c*/

	//select public parameters : p & alpha
	//p being a "safe prime" is an integer p which is such that both p and (p−1)/2 are prime
	//cuts on computational time if p is safeprime according to
	//https://crypto.stackexchange.com/questions/56155/primitive-root-of-a-very-big-prime-number-elgamal-ds

	mpz_t A;
	mpz_t B;
	mpz_t a;
	mpz_t b;

	mpz_init_set_str(a,"443154410456340133792289316319263982636340525614",10);
	mpz_init_set_str(b,"708552627548105121354432083524985769585714694203",10);

	mpz_init(A);
	mpz_init(B);
	mpz_t Z;
	mpz_init(Z);

	mpz_t message;
	mpz_t message_dec;
	mpz_init_set_str(message,"172201208208211",10);
	mpz_init(message_dec);
	mpz_t c1;
	mpz_t c2;
	mpz_init(c1);
	mpz_init(c2);
//generate parameters

generate_p_q_g(p,  q,  g);

	int base = 16;
	printf("\nUSING P : ");
	size_t  print_size0 = mpz_out_str(stdout ,base , p);//output p to stdout return 0 if error
	printf("\nMAIN:p,\nsize:%ld",print_size0);
	printf("\n\n");
	size_t  print_size1 = mpz_out_str(stdout ,base , q);//output p to stdout return 0 if error
	printf("\nMAIN:q,\nsize:%ld",print_size1);
	printf("\n\n");
	size_t  print_size2 = mpz_out_str(stdout ,base , g);//output p to stdout return 0 if error
	printf("\nMAIN:g,\nsize:%ld",print_size2);
	printf("\n\n");

//generate big X from little x
	generate_X_x(p, q, g, a , A);
	generate_X_x(p, q, g, b , B);
//generate Z from little x
	generate_Z_x(p, q, g, B , a, Z);
	generate_Z_x(p, q, g, A , b, Z);


//test vectors from : https://www.di-mgt.com.au/public-key-crypto-discrete-logs-1-diffie-hellman.html

	//string_to_decimal(str ,message);
struct timeval enc_start, enc_end; /* Used to compute elapsed time. */
struct timeval dec_start, dec_end; /* Used to compute elapsed time. */
int pass = 0;
signed long int cnt = 0;
signed long int y = 0;
time_t tim;
srand( (unsigned) time(&tim) );
signed long int random1 = rand();
signed long int random2 = rand();

for(y = 0; y < 100; y++){//TODO too long to run
	random1 = rand();
	random2 = rand();
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
encrypt_message(message, p, q , g, B, c1, c2);//encrypt from A to B
	gettimeofday(&enc_end, 0);
	mpz_t message_copy;
	mpz_init(message_copy);
	mpz_add(message_copy,message_copy,message);//0-> 0+x

	gettimeofday(&dec_start, 0); // start timer
decrypt_message(c1 , c2, p, q , g, b, message_dec);//decrypt a msesgae from A, by B
	gettimeofday(&dec_end, 0);
	//test if message_copy == message
	pass = mpz_cmp(message_copy,message_dec);
	if(pass == 0){
		printf("Time to encrypt: ");
		time_diff(&enc_start, &enc_end);
		printf("Time to decrypt: ");
		time_diff(&dec_start, &dec_end);

		send_output(c1, c2);

		cnt++;
	}
	mpz_clear(message_copy);
	mpz_clear(val);
}
printf("Passed %ld/%ld\n",cnt,y);


	//decimal_to_string(message, str);


	printf("\n\n");
	size_t  print_size6 = mpz_out_str(stdout ,base , message_dec);//output p to stdout return 0 if error
	printf("\nMAIN:message_dec:,\nsize:%ld",print_size6);
	printf("\n\n");


	mpz_clear(c1);
	mpz_clear(c2);
	mpz_clear(message);
	mpz_clear(message_dec);

	mpz_clear(g);
	mpz_clear(A);
	mpz_clear(B);
	mpz_clear(Z);
	mpz_clear(a);
	mpz_clear(b);
	return 0;
}

int main(int argc, char * argv[]) {
	mpz_t test_p;
	mpz_t test_q;
	mpz_t test_g;
	mpz_t test_j;
/*
	mpz_init_set_str(p,"19327210897467885519624495407304217845488409100133554803661172025039322784872775172789521895444178690740428588185031695453815386756662619555849446656794905221115788002016245291768283472480460523777510973085032471711187806590185987219179345022033106753600355795626394426859896564719805266547324204357196851217",10);
		//1932 test vector

	mpz_init_set_str(q,"983633858469108611936846792207646525014934079943",10);
//98 test vector
	//mpz_init_set_str(qq,"157662021640230088962448271877515019766202164023008896244827187751506546546544",10);
	//TODO remove hardcoded test vector
	mpz_init_set_str(g,"2008851267811649301382055697326002225321501629224616043097959307844472637339783779480891271906681929732776937543331689329117914118665148580824850572191418544875109802154341862162654424065963144063936607375606796563706389362731767772194368576684632589065496658911743756860379357301492526015846031839304359976",10);


	//print p & alpha
	unsigned long int oper_1 = 1;
	//mpz_t j;
	  mpz_init_set_str(j,"19648785705229832512739279235719869326994578857372093636756123297040568424679124645400612341931690579457038046947983396198878406475643907265436455907971703045747694698970488141521321179649598545010351796154042242157997633368825998252795580916414671992092563312",10);

*/
	mpz_init_set_str(test_p,"19327210897467885519624495407304217845488409100133554803661172025039322784872775172789521895444178690740428588185031695453815386756662619555849446656794905221115788002016245291768283472480460523777510973085032471711187806590185987219179345022033106753600355795626394426859896564719805266547324204357196851217",10);
	mpz_init_set_str(test_q,"983633858469108611936846792207646525014934079943",10);
	mpz_init_set_str(test_g,"2008851267811649301382055697326002225321501629224616043097959307844472637339783779480891271906681929732776937543331689329117914118665148580824850572191418544875109802154341862162654424065963144063936607375606796563706389362731767772194368576684632589065496658911743756860379357301492526015846031839304359976",10);
	mpz_init_set_str(test_j,"19648785705229832512739279235719869326994578857372093636756123297040568424679124645400612341931690579457038046947983396198878406475643907265436455907971703045747694698970488141521321179649598545010351796154042242157997633368825998252795580916414671992092563312",10);


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
		        case 'j':
		        	mpz_init_set_str(test_j,optarg,16);
		        	mpz_out_str(stdout, 16, test_p);//output
		        	break;

			}
		}

//test primality according to 'safe-prime' elgamal method
		printf("Testing primality of p: ");

		int prime_error_p = mpz_probab_prime_p(test_p,50);//test if prime
		int prime_error_q = mpz_probab_prime_p(test_q,50);//test if prime

		//0 = not prime; 1 = probably prime; 2 = definatley prime

		if(prime_error_p > 0 && prime_error_q > 0){
			printf("\nTEST result: POSITIVE\nRunning subsequent tests\n");
			ELGAMAL(test_p,test_q, test_g, test_j);
		}
		else{
			printf("\nTEST result: NEGATIVE\nTesting parameter generation\n");
			struct timeval para_start, para_end; /* Used to compute elapsed time. */
		
			gettimeofday(&para_start, 0);
			generate_p_q_g(test_p,test_q, test_g);//parameters
			gettimeofday(&para_end, 0);
			printf("Time to generate parameters: ");
			time_diff(&para_start, &para_end);

		
		}
		mpz_clear(test_p);
		mpz_clear(test_q);

		printf("Done with ElGamal\n");
		return 0;
}
