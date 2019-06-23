#include "ui.h"
#include "string.h"
int alu(int command, int operand){
    int t;
    int i, value = 0;

    switch(command){
        case ADD:
            sc_memoryGet(operand,&value);
            if (accumulator + value <= 0x7FFF)
                accumulator += value;
            else
                sc_regSet(P,1);
        break;
        case SUB:
            sc_memoryGet(operand, &value);
            if (accumulator - value <= 0x7FFF)
                accumulator-=value;
            else
                sc_regSet(P,1);
        break;
        case DIVIDE:
            sc_memoryGet(operand, &value);
            if (value == 0){
              sc_regSet(O,1);
              return 0;
            }
            if (accumulator/value<=0x7FFF)
                accumulator/=value;
        break;
        case MUL:
            sc_memoryGet(operand,&value);
            if (accumulator * value <= 0x7FFF)
                accumulator*=value;
            else
                sc_regSet(P,1);
        break;
    case(CHL):
        sc_memoryGet(operand, &t);
        accumulator = t<<1;
    break;
    case(SHR):
        sc_memoryGet(operand, &t);
        accumulator = t>>1;
    break;
    case(RCL):
        sc_memoryGet(operand, &t);
        if( t >> 14 & 0x1)
            accumulator = t>>1|0x4000;
        else
            accumulator = t>>1;
    break;
    case(RCR):
        sc_memoryGet(operand, &t);
        if( t & 0x1)
            accumulator = t<<1|0x4000;
        else
            accumulator = t<<1;
    break;
    case(NEG):
        sc_memoryGet(operand, &t);
    break;
    }

    return 0;
}
void initCU(int index){
  if(index<0||index>100)
    icounter = 0;
  else
    icounter = index;
}
void cu(){
    int command, operand;
    int value;
    if(sc_memoryGet(icounter,&value))
    {
      sc_commandDecode(value,&command,&operand);
      int t;
      int reg;
      switch(command){
          case(READ):
              sc_regGet(T,&reg);
              sc_regInit();
              sc_regSet(T, 1);
              clearIO();
              input("Input:", &t, true);
              sc_memorySet(operand, t);
              sc_regSet(T, reg);
          break;
          case(WRITE):
              sc_regInit();
              sc_memoryGet(operand, &t);
              sc_regSet(T, 1);
              clearIO();
              printf("Output[%d]: 0x%x, %d", operand, t, t);
              sc_regSet(T, 0);
          break;
          case(LOAD):
              sc_memoryGet(operand, &accumulator);
          break;
          case(STORE):
              sc_memorySet(operand, accumulator);
          break;
          case(ADD):case(SUB):case(DIVIDE):case(MUL):
              alu(command, operand);
          break;
          case(JUMP):
              icounter = operand;
          break;
          case(JNEG):
              if (accumulator < 0)
                  icounter = operand;
          break;
          case(JZ):
              if (accumulator == 0)
                  icounter = operand;
          break;
          case(HALT):
              sc_regSet(T, 1);
              icounter = operand;
              return;
          break;
      case(51):case(52):case(53):case(54):case(55):case(56):case(57):case(58):case(59):
      case(60):case(61):case(62):case(63):case(64):case(65):case(66):case(67):case(68):case(69):
      case(70):case(71):case(72):case(73):case(74):case(75):case(76):
          alu(command, operand);
      break;
      default:
          sc_regSet(T, 1);
          sc_regSet(E, 1);
      }
    }
    icounter++;
}
