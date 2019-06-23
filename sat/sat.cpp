#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
const int memSize = 100;
unsigned int sc_memory[memSize];

char operators[][7] = { "READ","WRITE","LOAD","STORE",
"ADD","SUB","DIVIDE","MUL",
"JUMP","JNEG","JZ","HALT",
"NOT","AND","OR","XOR","JNS","JC","JNC","JP","JNP",
"CHL","SHR","RCL","RCR","NEG","ADDC","SUBC","LOGLC","LOGRC","RCCL",
"RCCR","MOVA","MOVR","MOVCA","MOVCR","=" };
char *invalid_com_address = "Invalid command entry address",
     *invalid_com = "Unknow command",
     *invalid_oper_mb_plus = "Invalid operand.Possibly you forgot to add '+' before",
     *non_numeric_format = "Non numeric operand format",
     *indef_arg = "Indefinite argument";
int output (char const *filename)
{
	FILE *file;
	file = fopen (filename, "wb");
	if (!file) return 0;
	fwrite (sc_memory, sizeof(int), memSize, file);
	fclose (file);
	return 1;
}

int sc_commandEncode (int command, int operand, int *value)
{
    if (operand > 127) return 0;
    *value = (command << 7) + operand;
    return 1;
}

int c_error(char *err_msg,int row,char *lexeme,int code)
{
  printf("Error in row %d:%s \"%s\"\nError code:%d\n",row,err_msg,lexeme,code);
  exit(1);
}
int getCommandCode(char *lexeme)
{
  int i;
  bool flag = true;
  for (i = 0; i < 37&&flag; i++) {
    if(strcmp(operators[i],lexeme)==0)
    flag=false;
  }
  if(flag) return 0;
  switch (i-1) {
    case 0:return 10;
    case 1:return 11;
    case 2:return 20;
    case 3:return 21;
    case 4:return 30;
    case 5:return 31;
    case 6:return 32;
    case 7:return 33;
    case 8:return 40;
    case 9:return 41;
    case 10:return 42;
    case 11:return 43;
    case 12:return 51;
    case 13:return 52;
    case 14:return 53;
    case 15:return 54;
    case 16:return 55;
    case 17:return 56;
    case 18:return 57;
    case 19:return 58;
    case 20:return 59;
    case 21:return 60;
    case 22:return 61;
    case 23:return 62;
    case 24:return 63;
    case 25:return 64;
    case 26:return 65;
    case 27:return 66;
    case 28:return 67;
    case 29:return 68;
    case 30:return 69;
    case 31:return 70;
    case 32:return 71;
    case 33:return 72;
    case 34:return 73;
    case 35:return 74;
    case 36:return 1;
  }
  return 0;
}
bool isZero(char *num)
{
  std::string n(num);
  for (int i = 0; i < n.length(); i++) {
    if(num[i]!='\n')
    if(num[i]>'9'||num[i]<'0') return false;
  }
  return true;
}
int main(int argc, char const *argv[]) {
  if(argc != 3){
      printf("arg count:%d\n", argc);
      for(int i = 0;i<argc;i++)
        printf("%s\n", argv[i]);
      printf("Incorrect arguments");
      return 1;
  }
  for (int i=0;i<memSize;i++) sc_memory[i] = 0;
  FILE * input;
  input = fopen(argv[1],"r");
  if(!input){
    printf("%s not exist\n", argv[0]);
    return 1;
  }
  char *expr =(char*) calloc(100,sizeof(char));
  int iterator = 1,row = 1;
  int address;
  int command;
  int operand;
  do {
    fgets(expr,100,input);
    if(!feof(input))
    {
      char * tok = strtok(expr," ");
      while (tok!=NULL) {
        switch (iterator) {
          case 1:{
                 if(!(address = atoi(tok))){
                   if(!isZero(tok))
                    c_error(invalid_com_address,row,tok,1);
                 }
                 break;
               }
          case 2:{
                  if(!(command = getCommandCode(tok)))
                   c_error(invalid_com,row,tok,2);
                  break;
                }
          case 3:{
                  if(command==1)
                  {
                    printf("%s\n",tok);
                    if(tok[0]!='+')
                    {
                      c_error(invalid_oper_mb_plus, row, tok, 31);
                    } else{
                      if(!(operand = atoi(&tok[1]))){
                      if(!isZero(&tok[1]))
                       c_error(non_numeric_format,row,tok,32);
                      }
                      printf("o:%d\n",operand);
                    }
                  } else{
                    if(!(operand = atoi(tok))){
                    if(!isZero(tok))
                     c_error(non_numeric_format,row,tok,33);
                    }
                  }
                  break;
                }
          case 4:{
                  if(tok[0]!=';') {
                    char indarg[100];
                    while(tok!=NULL)
                    {
                      strcat(indarg,tok);
                    }
                    c_error(indef_arg, row,indarg,4);
                  }
                }
      }
      tok = strtok(NULL," ");
      iterator++;
    }
    if(command!=1)
    {
      int value;
      sc_commandEncode(command,operand,&value);
      sc_memory[address] = value;
    } else {
      sc_memory[address] = operand;
    }
    iterator = 1;
    row++;
   }
  } while(!feof(input));
  fclose(input);
  output(argv[2]);
  printf("\nCompiled successfully\n");
  for (int i = 0; i < 100; i++) {
    if(i%10==0) printf("\n");
    printf("%d\t",sc_memory[i]);
  }
  return 0;
}
