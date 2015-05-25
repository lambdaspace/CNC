//http://linuxcnc.org/docs/html/gcode.html
//http://www.nongnu.org/avr-libc/
//http://bildr.org/2011/06/easydriver/#
//http://www.sparkfun.com/datasheets/Robotics/A3967.pdf
//http://danthompsonsblog.blogspot.com/2010/05/easydriver-42-tutorial.html
//http://schmalzhaus.com/EasyDriver/EasyDriver_v44/EasyDriver_v44_sch.pdf
//http://lusorobotica.com/index.php/topic,106.0.html
//http://wiki.linuxcnc.org/cgi-bin/wiki.pl?Stepper_Motor_Speed_Limitations
//http://schmalzhaus.com/EasyDriver/Examples/EasyDriverExamples.html
//http://www.daycounter.com/Calculators/Stepper-Motor-Calculator.phtml

//http://www.visualmicro.com/


#include "StepperMotor.h"
#include "PuntsList.h"

#define SERIAL_BUFFER_LENGTH 255                     // en caracters
#define SERIAL_TIMEOUT 100                           // en ms
#define INFORMA_DELAY 100                            // Temps entre enviaments de la posició de la CNC al host en ms

#define NUL  0                                       // Final d'string
#define HLT  1                                       // Halt. Stop ja (parada d'emergència)
#define UHLT 2                                       // UnHalt CNC  reanuda el programa
#define RST  3                                       // Reset ja
#define IAH  5                                       // I Am Here
#define ACK  6                                       // Acknowledge
#define ACKC 7                                       // Acknowledge Critic (correspon a una comande de control urgent)
#define BSY  8                                       // Busy
#define RDY  9                                       // Ready
#define LF  10                                       // LineFeed  '\n' = 10 = LF = NewLine
#define NAC 11                                       // Not Acceped Command
#define ABRT 12                                      // Aborta programa
//#define CR 13										 // Carriage Return
//#define HM 14										 // Home
//#define AHM 15										 // Absolute Home
#define END 16										 // El programa ha acabat
#define PLY 17										 // CNC is Playing

#define SP_STOP 0                                    // Stop
#define SP_CW   1                                    //Turn spindle clockwise           
#define SP_CCW -1                                    //Turn spindle counterclockwise 

boolean busy = false;                                // Buffer ple?
boolean busyChanged =false;                          // hi ha lloc al buffer?
boolean hostFound = false;                           // hi ha un ordinador al l'altre banda?
boolean programStarted = false;						 // si ha començat algun programa

boolean cncStepsLeft = false;                            // pot processar el següent punt?
boolean rapidMotion =false;                          // moviment no sincronitzat? 'G0' no sincronitzat, 'G1' sincronització linial
boolean absolutes = true;                            // coordenades absolutes? 'G90' absolutes, 'G91' relatives
boolean enMilimetres = true;                         // coordenades en mm? mm 'G21' , polsades 'G20'  
int spindle = SP_STOP;                               // estat de l'spindle (o laser o.. )
boolean cncDelay = false;							// si true espera 1s abans de processar la propera comanda

long millLastDialog =0;                              // la darrera vegada que hem parlat amb host
// si pasa mes de SERIAL_TIMEOUT intentem tornar a conectar

PuntsList punts;                                     // llista de punts pendents de processar
Punt target;                                         // punt actual

const int eixX = 0;
const int eixY = 1;
const int eixZ = 2;
StepperMotor motor[3];                               // Motrors de la CNC


void setup() {
    initCNC();

    Serial.begin(115200);                              // Initialize the Serial port:
    Serial.setTimeout(SERIAL_TIMEOUT);
    Serial.flush();
}////

long abc = 0;

void loop(){
    imReady();
    areYouThere();
    runNextPoint();
//Serial.print("# ");  Serial.print(abc,DEC);  Serial.print("  \n");  abc++;
}////




