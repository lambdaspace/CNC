#include <avr/wdt.h>


void parseCommand(char *inChars){
    byte char0;
    String inString ="                                        ";
	float mm[3] = {    0.0, 0.0, 0.0     };

    inString = inChars;       // inString = inString.copy(inChars, strlen(inChars));   
    char0 = (byte)inString.charAt(0);
    switch(char0){
    case ACK:  
        Serial.print("<ACK  \n");                            
        break;
    case RDY:  
        Serial.print("<RDY  \n");                            
        break;
    case IAH:  
        Serial.print("<IAH  \n");   
        hostFound = true;        
        break;
    case ABRT:  
        sendToHost(ACKC);
        Serial.print("<ABRT  \n");Serial.print("#ABRT  \n");
        for(int n = 0; n<3; n++) motor[n].halt();
		for(int n = 0; n<3; n++) motor[n].abort();
		busy = false;                                // Buffer ple?
		busyChanged =false;                          // hi ha lloc al buffer?
		hostFound = false;                           // hi ha un ordinador al l'altre banda?

		cncStepsLeft = false;                            // pot processar el següent punt?
		rapidMotion =false;                          // moviment no sincronitzat? 'G0' no sincronitzat, 'G1' sincronització linial
		absolutes = true;                            // coordenades absolutes? 'G90' absolutes, 'G91' relatives
		enMilimetres = true;                         // coordenades en mm? mm 'G21' , polsades 'G20'  
		spindle = SP_STOP;                               // estat de l'spindle (o laser o.. )
		cncDelay = false;							// si true espera 1s abans de processar la propera comanda
		punts.resetPunts();
		for(int n = 0; n<3; n++) motor[n].unHalt();
        break;  // Aborta el programa actual
    case HLT:  
        sendToHost(ACKC);
        Serial.print("<HLT  \n");Serial.print("#HLT  \n");
        for(int n = 0; n<3; n++) motor[n].halt();    
        break;  // parada d'emeregència
    case UHLT: 
        sendToHost(ACKC);
        Serial.print("<UHLT  \n");Serial.print("#UHLT  \n");   
        for(int n = 0; n<3; n++) motor[n].unHalt();  
        break;  // reanuda la parada d'emeregència
  //  case HM:  
  //      sendToHost(ACKC);
		//for(int n = 0; n<3; n++) motor[n].abort();
		//punts.resetPunts();
		//mm[0] = 0.0; mm[1] = 0.0; mm[2] = 10.0;
		//punts.pushPunt(mm, true, true, false, 0, false, false);
  //      break;  
  //  case AHM:  
  //      sendToHost(ACKC);
		//for(int n = 0; n<3; n++) motor[n].abort();
		//punts.resetPunts();
		//mm[0] = -300.0; mm[1] = -300.0; mm[2] = 300.0;
		//punts.pushPunt(mm, false, true, false, 0, false, false);
  //      break;   
    case RST:  
        sendToHost(ACKC);
        sendToHost(BSY);         // Reset de la CNC
        Serial.print("<RST  \n");Serial.print("#RST  \n");
		//softReset();
        //asm volatile ("  jmp 0X3C00  \n\t");
		softReset();
        break;  
    case 'G':  
        gParse(inString);       
        break;
    case 'M':  
        mParse(inString);       
        break;
    default:   
        sendToHost(ACK);          
        break;
    }
}


void softReset(){ 
  wdt_disable();  
  wdt_enable(WDTO_15MS);
  while (1) {}
//
//  //asm volatile ("  jmp 0  \n\t");
//  //asm volatile ("  jmp 0X3C00  \n\t");  // salta a la direcció del reset vactor plana 291
}     // Restarts program from beginning but does not reset the peripherals and registers


