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
	START1,//0
	OP1,//1
	VALID1,//2
	BLUE1,//3
	GREEN1,//5
	PROC1,//4
	STRET1,//6
	RETR1,//7
	FIN1,//8
} stateNames4;

// "Tapetes_2" state machine
typedef enum{
	START2,
	OP2,
	VALID2,
	BLUE2,
	GREEN2,
	PROC2,
	STRET2,
	RETR2,
	FIN2,
} stateNames5;

// Timers
typedef struct {
    bool on;
    uint64_t time;
} Timer;

// cycle time
uint64_t  start_time = 0, end_time = 0, cycle_time = 0;
/* uint64_t  stoptime = 10000; // 10sec */

Timer timetap, timeout, timewait;

void update_timers(){

	end_time = get_time();

	if (start_time == 0)
		cycle_time = start_time;
	else 
		cycle_time = end_time - start_time;

	start_time = end_time;

	if (timetap.on)
		timetap.time = timetap.time + cycle_time;		

	if (timeout.on)
		timeout.time = timeout.time + cycle_time;
	
	if (timewait.on)
		timewait.time = timewait.time + cycle_time;
	
}

void start_timer(Timer* t){
	t->on = true;
	t->time = 0;
}

void stop_timer(Timer* t){
	t->on = false;
	t->time = 0;
}

// Funções
void init_SM();

// State machines current state
stateNames1 currentState1 = Parado;
stateNames2 currentState2 = LOFF;
stateNames3 currentState3 = INIT;
stateNames4 currentState4 = START1;
stateNames5 currentState5 = START2;

// Falling edges
bool fe_START = false, fe_STOP = false, fe_ST2 = false, fe_ST3 = false, fe_STR1 = false, fe_STR2 = false;

// Previous values
bool p_START = 0,  p_STOP = 0, p_ST2 = 0, p_ST3 = 0, p_STR1 = 0, p_STR2 = 0;

void edges(){
	// Detects edges

	// FALLING EDGES
	if(p_START == true && START == false)	
		fe_START = true;
	else fe_START = false;

	if(p_STOP == true && STOP == false)	
		fe_STOP = true;
	else fe_STOP = false;

	if(p_ST2 == true && ST2 == false)	
		fe_ST2 = true;
	else fe_ST2 = false;

	if(p_ST3 == true && ST3 == false)	
		fe_ST3 = true;
	else fe_ST3 = false;

	if(p_STR1 == true && STR1 == false)	
		fe_STR1 = true;
	else fe_STR1 = false;

	if(p_STR2 == true && STR2 == false)	
		fe_STR2 = true;
	else fe_STR2 = false;

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
				
				// Transition A_Parar -> Tapete
				if (SV1 == 0 && SV2 == 0){
					start_timer(&timetap);
					if (timetap.time >= 10000){
						// Next State
						currentState1 = Tapete;
					}
						
				}
			break;

			case Tapete :
				stop_timer(&timetap);
				start_timer(&timeout);
				// Transition Tapete -> Parado
				if (timeout.time >= 15000)
					// Next State
					currentState1 = Parado;
					
			break;
	} //end case

}

void wait(){
	// sm controls wait
	switch (currentState2) {
			case LOFF :
				start_timer(&timewait);
				// Transition LOFF -> LON
				if ((currentState1 == A_Parar) && timewait.time >= 2000)
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
			
			else if (fe_ST2 == true)
				currentState3 = BLUE;
			
		break;

			AZUIS = 0;
			VERDES = 0;

		case GREEN :
			//Transition GREEN -> INIT or counts GREEN
			if (fe_ST3 == true)
				VERDES++;

			else if ((fe_START == 1) && currentState1 == Parado)
				currentState3 = INIT;

		break;

		case BLUE : 
			//Transition BLUE -> or counts BLUE
			if (fe_ST2 == true)
				AZUIS++;

			else if ((fe_START == 1) && currentState1 == Parado)
				currentState3 = INIT;

		break;
	}


}

void Tapetes_1(){
	// sm controls tapetes
	switch(currentState4) {
		case START1 : 
			if (currentState1 == Operar)
				currentState4 = OP1;

		break;
		
		case OP1 :
			if (SV1 != 0)
				currentState4 = VALID1;
			
			else if (currentState1 == Tapete)
				currentState4 = START1;

		break;
		
		case VALID1 :
			if (SV1 == 1 && SV2 != 1)
				currentState4 = BLUE1;

			else if (SV1 == 4)
				currentState4 = GREEN1;

		break;
		
		case BLUE1 :
			if (fe_STR1 == 1)
				currentState4 = PROC1;

		break;
		
		case PROC1 :
			if (fe_ST2 == 1)
				currentState4 = START1;

		break;

		case GREEN1 :
			if (fe_STR1 == 1)
				currentState4 = STRET1;
				
		break;
			
		case STRET1 :
			if (SPE1 == 1)
				currentState4 = RETR1;

		break;
			
		case RETR1 :
			if (SPR1 == 1)
				currentState4 = FIN1;

		break;
		
		case FIN1 :
			if (fe_ST3 == 1)
				currentState4 = START1;

		break;
			
	}
}

void Tapetes_2(){
	// sm controls tapetes
	switch(currentState5) {
		case START2 : 
			if (currentState1 == Operar)
				currentState5 = OP2;

		break;
		
		case OP2 :
			if (SV2 != 0)
				currentState5 = VALID2;
				
			else if (currentState1 == Tapete)
				currentState5 = START2;

		break;
		
		case VALID2 :
			if (SV2 == 4 && SV1 != 4)
				currentState5 = GREEN2;

			else if (SV1 != 4 && SV2 == 1)
				currentState5 = BLUE2;

		break;
		
		case GREEN2 :
			if (fe_STR2 == 1)
				currentState5 = PROC2;

		break;
		
		case PROC2 :
			if (fe_ST3 == 1)
				currentState5 = START2;

		break;

		case BLUE2 :
			if (fe_STR2 == 1)
				currentState5 = STRET2;
				
		break;
			
		case STRET2 :
			if (SPE2 == 1)
				currentState5 = RETR2;

		break;
			
		case RETR2 :
			if (SPR2 == 1)
				currentState5 = FIN2;

		break;
		
		case FIN2 :
			if (fe_ST2 == 1)
				currentState5 = START2;

		break;
				
	}
}

// Initializes state machine
void init_SM()
{
	printf("\n *** Initializing ***");
}


// Código principal
int main() {
	
	// Inicialização da ME
	init_SM();
	
	// Ciclo de execução
	while(1) {

		// Updates timers
		update_timers();

		#ifdef DEBUG
		printf ("\n*** Inicio do Ciclo ***\n");
		#endif
	

		// State machines
		operacao();
		wait();
		contador();		
		Tapetes_1();
		Tapetes_2();

		// Leitura das entradas
		read_inputs();

		// Edge detection
		edges();

		LSTOP = (currentState1 == Parado);
		LSTART = (currentState1 == Operar);
		E1 = (currentState1 == Operar);
		E2 = (currentState1 == Operar);
		T3A = (currentState1 == Tapete);
		LWAIT = (currentState2 == LON);
		T1A = ((currentState4 == OP1) || (currentState4 == BLUE1)) || (currentState4 == GREEN1);
		T2A = ((currentState1 == Tapete) || (currentState4 == GREEN1) || (currentState4 == BLUE1) || (currentState4 == PROC1)|| (currentState5 == FIN2));
		T3A = ((currentState4 == FIN1) || (currentState1 == Tapete) || (currentState5 == BLUE2) || (currentState5 == GREEN2) || (currentState5 == PROC2));
		T4A = ((currentState5 == OP2) || (currentState5 == BLUE2)) || (currentState5 == GREEN2);
		PE1 = (currentState4 == STRET1);
		PR1 = (currentState4 == RETR1);
		PE2 = (currentState5 == STRET2);
		PR2 = (currentState5 == RETR2);

		//Escrita nas saídas
		write_outputs();

		//sleep_abs(scan_time);
		
	} // end loop
	
} // end main