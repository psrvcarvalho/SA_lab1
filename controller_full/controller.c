// ---------- Máquina de estados -----------

#include "IO.c"


#undef DEBUG

// "Operação" state machine
typedef enum{
	Parado,
	Operar,
	A_Parar,
	Tapete,
} stateNames1;

// "Wait" state machine
typedef enum{
	LOFF,
	LON,
} stateNames2;

// "Contador" state machine
typedef enum{
	INIT,
	GREEN,
	BLUE,
} stateNames3;

// "Tapetes_1" state machine
typedef enum{
	TSTART1;
	TOP1;
	TVALID1;
	TBLUE1;
	TGREEN1;
	TPROC1;
	TSTRETCH1;
	TRETREAT1;
	TFIN1;
} stateNames4;

// "Tapetes_2" state machine
typedef enum{
	TSTART2;
	TOP2;
	TVALID2;
	TBLUE2;
	TGREEN2;
	TPROC2;
	TSTRETCH2;
	TRETREAT2;
	TFIN2;
} stateNames5

/*
// Timers
typedef struct {
    uint32_t start_time,
    uint32_t timeout,
    bool active,
} Timer;
*/



// Funções
void init_SM();

// State machines current state
stateNames1 currentState1 = Parado;
stateNames2 currentState2 = LOFF;
stateNames3 currentState3 = INIT;
stateNames4 currentState4 = TSTART1;
stateNames5 currentState5 = TSTART2;

// cycle time
uint64_t  scan_time = 1000;	// 1sec
/* uint64_t  stoptime = 10000; // 10sec */

// Falling edges
bool fe_START = False, fe_STOP = False, fe_ST2 = False, fe_ST3 = False, fe_STR1 = False, fe_STR2 = False;

// Previous values
bool p_START = 0,  p_STOP = 0, p_ST2 = 0, p_ST3 = 0, p_STR1 = 0, p_STR2 = 0;

void edges(){
	// Detects edges

	// FALLING EDGES
	if(p_START == true && START = False)	
		fe_START = true;
	else fe_START = False;

	if(p_STOP == true && STOP = False)	
		fe_STOP = true;
	else fe_STOP = False;

	if(p_ST2 == true && ST2 = False)	
		fe_ST2 = true;
	else fe_ST2 = False;

	if(p_ST3 == true && ST3 = False)	
		fe_ST3 = true;
	else fe_ST3 = False;

	if(p_STR1 == true && STR1 = False)	
		fe_STR1 = true;
	else fe_STR1 = False;

	if(p_STR2 == true && STR2 = False)	
		fe_STR2 = true;
	else fe_STR2 = False;

	// updates values
	p_START = START;
	p_STOP = STOP;
	p_ST2 = ST2;
	p_ST3 = ST3;
	p_STR1 = STR1;
	p_STR2 = STR2;
	
}

void operacao(){
	// sm controls operacao
	switch (currentState1) {
			case Parado :
					
				// Transition Parado -> Operar
				if (fe_START == 1)
					// Next State
					currentState1 = Operar;
			
					break;
			
			case Operar :
				
				// Transition Operar -> A_Parar
				if (fe_STOP == 1)
					// Next State
					currentState1 = A_Parar;
	
					break;

			case A_Parar :
				
				/*
				// Initialize the timer to 10 seconds
    			sm->timer.start_time = millis();
    			sm->timer.timeout = 10000;
    			sm->timer.active = true;
				*/

				// Transition A_Parar -> Tapete
				if ((timer1.time >= 10000) && (SV1 == 0 || SV2 == 0))
					// Next State
					currentState1 = Tapete;
					
					break;

			case Tapete :
				
				// Transition Tapete -> Parado
				if (timer1.time >= 15000)
					// Next State
					currentState1 = Parado;
					
					break;
	} //end case

}

void wait(){
	// sm controls wait
	switch (currentState2) {
			case LOFF :
				// Transition LOFF -> LON
				if ((currentState1 == A_Parar) && timer2.time >= 2000)
					currentState2 = LON;

					break;

			case LON :
				// Transition LON -> LOFF
				currentState2 = LOFF;

				break;
	}


}

void contador(){
	// sm controls contador
	switch (currentState3) {
		case INIT :
			//Transistion INIT -> GREEN or INIT -> BLUE
			if (fe_ST3 == true)
				currentState3 = GREEN;

				break;
			
			else if (fe_ST2 == true)
				currentState3 = BLUE;
			
				break;

			AZUIS = 0;
			VERDES = 0;

		case GREEN :
			//Transition GREEN -> INIT or counts GREEN
			if (fe_ST3 == true)
				VERDES++;

				break;

			else if ((fe_START == 1) && currentState1 == Parado)
				currentState3 = INIT;

				break;

		case BLUE : 
			//Transition BLUE -> or counts BLUE
			if (fe_ST2 == true)
				AZUIS++;

				break;

			else if ((fe_START == 1) && currentState1 == Parado)
				currentState3 = INIT;

				break;
	}


}

void tapetes(){
	// sm controls tapetes
	switch(currentState4) {
		case TSTART1 : 
			if (currentState1 == Operar)
				currentState4 = TOP1;

				break;
		
		case TOP1 :
			if (SV1 != 0)
				currentState4 = TVALID1;

				break;
		
		case TVALID1 :
			if (SV1 == 1 && SV2 != 1)
				currentState4 = TBLUE1;

				break;

			else if (SV1 == 4)
				currentState4 = TGREEN1;

				break;
		
		case TBLUE1 :
			if (fe_STR1 == 1)
				currentState4 = TPROC1;

				break;
		
		case TPROC1 :
			if 
				
	}
}

// Initializes state machine
void init_SM()
{
	printf("\n *** Initializing ***")
}

// Código principal
int main() {

	
	// Inicialização da ME
	init_SM();
	
	// Ciclo de execução
	while(1) {

		#ifdef DEBUG
		printf ("\n*** Inicio do Ciclo ***\n");
		#endif

		//timer1
		start_timer(&timer1);
		stop_timer(&timer1);


		// State machines
		operacao();
		wait();
		contador();		
		tapetes();

		// Leitura das entradas
		read_inputs();

		// Edge detection
		edges();

		// Updates timers
		update_timers();

		// Atualiza saídas

		// Saídas booleanas
		LSTOP = (currentState1 == Parado);
		LSTART = (currentState1 == Operar);
		E1 = (currentState1 == Operar);
		E2 = (currentState1 == Operar);
		T2A = (currentState1 == Tapete);
		T3A = (currentState1 == Tapete);
		LWAIT = (currentState2 == LON);
		T1A = (currentState4 == TOP1);
		
		//Escrita nas saídas
		write_outputs();

		
		//Aguarda pelo próximo ciclo
		sleep_abs(scan_time);
		
	} // end loop
	
} // end main