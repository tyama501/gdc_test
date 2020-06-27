#include <stdio.h>
#include <dos.h>

char  _asm_1(char *);
char  _asm_2(char *, char); 
char  _asm_3();
#define inpA0h()        _asm_1("\n\tIN\tAL,160\n")
#define outA0h(odata)   _asm_2("\n\tOUT\t160,AL\n", odata)
#define outA2h(odata)   _asm_2("\n\tOUT\t162,AL\n", odata)
#define out6Ah(odata)   _asm_2("\n\tOUT\t106,AL\n", odata)

void waitF_empty(void);
int gdc_status_rd(void);
void gdc_reset(void);
void gdc_start(void);
void gdc_stop(void);
void gdc_vectw_dot(char dir, int circle_r);
void gdc_textw(void);
void gdc_csrw(int x, int y);
void gdc_write(void);
void gdc_vecte(void);
void color16(char odata);

union REGS reg1;

void waitF_empty(void)
{
  while(!(gdc_status_rd() & 0x04)){};
  printf("FIFO is empty.\n");
}

int gdc_status_rd(void)
{
  reg1.h.al = inpA0h();
  printf("Input from port A0h : GDC Status Read : %x\n", reg1.h.al);

  return reg1.h.al; 
}

void gdc_reset(void)
{
  reg1.h.al = 0x00;
  outA2h(reg1.h.al);
  printf("Output to port A2h : GDC RESET : %x\n", reg1.h.al);
}

void gdc_start(void)
{
  reg1.h.al = 0x0d;
  outA2h(reg1.h.al);
  printf("Output to port A2h : GDC START : %x\n", reg1.h.al);
}

void gdc_stop(void)
{
  reg1.h.al = 0x0c;
  outA2h(reg1.h.al);
  printf("Output to port A2h : GDC STOP : %x\n", reg1.h.al);
}

void gdc_vectw_dot(char dir, int circle_r)
{
  char p1;
  int p5_p4;
  int p7_p6;
  int num_dot;

  p1 = 0x20 | dir;  /* CIRCLE */
  num_dot = 2 * 3.14 * circle_r;
  p5_p4 = circle_r - 1;
  p7_p6 = 2 * p5_p4;

  reg1.h.al = 0x4c;
  outA2h(reg1.h.al);
  printf("Output to port A2h : GDC VECTW : %x\n", reg1.h.al);

  reg1.h.al = p1;   /* P1 */
  outA0h(reg1.h.al);
  reg1.h.al = 0xFF & num_dot;   /* P2 */
  outA0h(reg1.h.al);
  reg1.h.al = (0x3F00 & num_dot) >> 8;   /* P3 */
  outA0h(reg1.h.al);
  reg1.h.al = 0xFF & p5_p4;   /* P4 */
  outA0h(reg1.h.al);
  reg1.h.al = (0x3F00 & p5_p4) >> 8;   /* P5 */
  outA0h(reg1.h.al);
  reg1.h.al = 0xFF & p7_p6;   /* P6 */
  outA0h(reg1.h.al);
  reg1.h.al = (0x3F00 & p7_p6) >> 8;   /* P7 */
  outA0h(reg1.h.al);
  reg1.h.al = 0xFF;   /* P8 */
  outA0h(reg1.h.al);
  reg1.h.al = 0xFF;   /* P9 */
  outA0h(reg1.h.al);
  reg1.h.al = 0x00;   /* P10 */
  outA0h(reg1.h.al);
  reg1.h.al = 0x00;   /* P11 */
  outA0h(reg1.h.al);
}

void gdc_textw(void)
{
  reg1.h.al = 0x78;
  outA2h(reg1.h.al);
  printf("Output to port A2h : GDC TEXTW : %x\n", reg1.h.al);

  reg1.h.al = 0xFF;   /* P1 */
  outA0h(reg1.h.al);
  reg1.h.al = 0xFF;   /* P2 */
  outA0h(reg1.h.al);
  reg1.h.al = 0x00;   /* P3 */
  outA0h(reg1.h.al);
  reg1.h.al = 0x00;   /* P4 */
  outA0h(reg1.h.al);
  reg1.h.al = 0x00;   /* P5 */
  outA0h(reg1.h.al);
  reg1.h.al = 0x00;   /* P6 */
  outA0h(reg1.h.al);
  reg1.h.al = 0x00;   /* P7 */
  outA0h(reg1.h.al);
  reg1.h.al = 0x00;   /* P8 */
  outA0h(reg1.h.al);
}

void gdc_csrw(int x, int y)
{
  long byte_addr;
  char bit_addr;

  byte_addr = y * 80 + x / 8;
  bit_addr = x % 8;

  reg1.h.al = 0x49;
  outA2h(reg1.h.al);
  printf("Output to port A2h : GDC CSRW : %x\n", reg1.h.al);

  reg1.h.al = 0xFF & byte_addr;   /* P1 */
  outA0h(reg1.h.al);
  reg1.h.al = (0xFF00 & byte_addr) >> 8;   /* P2 */
  outA0h(reg1.h.al);
  reg1.h.al = ((0x0F & bit_addr) << 4) | ((0x030000 & byte_addr) >> 16);/* P3 */
  outA0h(reg1.h.al);
}

void gdc_write(void)
{
  reg1.h.al = 0x20;
  outA2h(reg1.h.al);
  printf("Output to port A2h : GDC WRITE : %x\n", reg1.h.al);
}

void gdc_vecte(void)
{
  reg1.h.al = 0x6c;
  outA2h(reg1.h.al);
  printf("Output to port A2h : GDC VECTE : %x\n", reg1.h.al);
}

void color16(char odata)
{
  reg1.h.al = odata;
  out6Ah(reg1.h.al);
  printf("Output to port 6Ah : Mode F/F Register 2 : %x\n", reg1.h.al);
}

int main(void)
{

  color16(0x01);

  waitF_empty();
  gdc_stop();
  waitF_empty();
  gdc_reset();
  waitF_empty();
  gdc_start();
  waitF_empty();

  gdc_textw();
  waitF_empty();
  gdc_write();
  waitF_empty();

  gdc_csrw(100,100);
  waitF_empty();
  gdc_vectw_dot(0,100);
  waitF_empty();
  gdc_vecte();
  waitF_empty();

  gdc_csrw(200,0);
  waitF_empty();
  gdc_vectw_dot(1,100);
  waitF_empty();
  gdc_vecte();
  waitF_empty();

  gdc_csrw(200,200);
  waitF_empty();
  gdc_vectw_dot(2,100);
  waitF_empty();
  gdc_vecte();
  waitF_empty();

  gdc_csrw(100,100);
  waitF_empty();
  gdc_vectw_dot(3,100);
  waitF_empty();
  gdc_vecte();
  waitF_empty();

  gdc_csrw(300,100);
  waitF_empty();
  gdc_vectw_dot(4,100);
  waitF_empty();
  gdc_vecte();
  waitF_empty();

  gdc_csrw(200,200);
  waitF_empty();
  gdc_vectw_dot(5,100);
  waitF_empty();
  gdc_vecte();
  waitF_empty();

  gdc_csrw(200,0);
  waitF_empty();
  gdc_vectw_dot(6,100);
  waitF_empty();
  gdc_vecte();
  waitF_empty();

  gdc_csrw(300,100);
  waitF_empty();
  gdc_vectw_dot(7,100);
  waitF_empty();
  gdc_vecte();
  waitF_empty();

  return 0;
}
