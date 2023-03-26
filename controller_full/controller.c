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

Timer timetap, timeout, timewait, timeed;

void update_timers(){

	end_time = get_time();

	if (start_time == 0)
		cycle_time = start_time;
	else 
		cycle_time = end_time - start_time;

	start_time = end_time;
	//printf("%lu", get_time());
	//printf("\nstarttime: %lu\ncycletime: %lu\nend_time: %lu\n", start_time, end_time, cycle_time);		

	if (timetap.on){

		timetap.time = timetap.time + cycle_time;
		//printf("timetap:%d***%lu***\n", timetap.on, timetap.time);
	}

	if (timeout.on){

		timeout.time = timeout.time + cycle_time;
		//printf("timeout:%d***%lu***\n", timeout.on, timeout.time);
	}
	
	if (timewait.on){

		timewait.time = timewait.time + cycle_time;
		//printf("timewait:%d***%lu***\n", timewait.on, timewait.time);
	}

	if (timeed.on){

		timeed.time = timeed.time + cycle_time;
	}
	
}

void start_timer(Timer* t){
	t->on = true;
	t->time = 0;
}

void stop_timer(Timer* t){
	t->on = false;
	t->time = 0;
}

stateNames1 currentState1;
stateNames2 currentState2;
stateNames3 currentState3;
stateNames4 currentState4;
stateNames5 currentState5;

// State machines next state
stateNames1 nextState1;
stateNames2 nextState2;
stateNames3 nextState3;
stateNames4 nextState4;
stateNames5 nextState5;

// Funções
//void init_SM();
// Initializes state machine
void init_SM()
{
	printf("\n *** Initializing ***");
	// State machines current state
	currentState1 = Parado;
	currentState2 = LOFF;
	currentState3 = INIT;
	currentState4 = START1;
	currentState5 = START2;

}


// Falling edges
bool fe_START = 0, fe_STOP = 0, fe_ST2 = 0, fe_ST3 = 0, fe_STR1 = 0, fe_STR2 = 0;

// Previous values
bool p_START = 0,  p_STOP = 0, p_ST2 = 0, p_ST3 = 0, p_STR1 = 0, p_STR2 = 0;
// Previous values will identify if pieces are still between sensors
int p_SV1 = 0, p_SV2 = 0;

void edges(){
	// Detects edges

	// FALLING EDGES
	if(p_START == 1 && START == 0)	
		fe_START = 1;
	else fe_START = 0;

	if(p_STOP == 1 && STOP == 0)	
		fe_STOP = 1;
	else fe_STOP = 0;

	if(p_ST2 == 1 && ST2 == 0)	
		fe_ST2 = 1;
	else fe_ST2 = 0;

	if(p_ST3 == 1 && ST3 == 0)	
		fe_ST3 = 1;
	else fe_ST3 = 0;

	if(p_STR1 == 1 && STR1 == 0)	
		fe_STR1 = 1;
	else fe_STR1 = 0;

	if(p_STR2 == 1 && STR2 == 0)	
		fe_STR2 = 1;
	else fe_STR2 = 0;

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
				if (fe_START == 1){
					// Next State
					nextState1 = Operar;
					AZUIS = 0;
					VERDES = 0;
				}
			
			break;
			
			case Operar :
				
				// Transition Operar -> A_Parar
				if (fe_STOP == 1){
					// Next State
					nextState1 = A_Parar;
				}
	
			break;

			case A_Parar :
				
				// Transition A_Parar -> Tapete
				if (SV1 == 0 && SV2 == 0){
					if (timetap.on == 0){
						start_timer(&timetap);
					}
					if (timewait.on == 0){
						start_timer(&timewait);
					}
					if (timetap.time >= 10000){
						// Next State
						nextState1 = Tapete;
						stop_timer(&timetap);
						stop_timer(&timewait);
					}
						
				}
				
			break;

			case Tapete :
				if(timeout.on == 0){
					start_timer(&timeout);
				}
				// Transition Tapete -> Parado
				if (timeout.time >= 15000){
					// Next State
					nextState1 = Parado;
					stop_timer(&timeout);
				}
					
			break;
	} //end case

}

void wait(){
	// sm controls wait
	switch (currentState2) {
			case LOFF :
				// Transition LOFF -> LON
				if (currentState1 == A_Parar && timewait.time >= 2000){
						nextState2 = LON;
						start_timer(&timeed);
				}

			break;

			case LON :
				// Transition LON -> LOFF
				if(timeed.time >= 2000){
					nextState2 = LOFF;
					stop_timer(&timeed);
					start_timer(&timewait);
				}


		break;
	}


}

void contador(){
	// sm controls contador
	switch (currentState3) {
		case INIT :
			//Transistion INIT -> GREEN or INIT -> BLUE
			if (fe_ST3 == true)
				nextState3 = GREEN;
			
			else if (fe_ST2 == true)
				nextState3 = BLUE;
			
		break;

		case GREEN :
			VERDES++;
			nextState3 = INIT;
		break;

		case BLUE : 
			AZUIS++;
			nextState3 = INIT;

		break;
	}


}

void Tapetes_1(){
	// sm controls tapetes
	switch(currentState4) {
		case START1 : 
			if (currentState1 == Operar)
				nextState4 = OP1;

		break;
		
		case OP1 :
			if (SV1 != 0)
				nextState4 = VALID1;
			
			else if (currentState1 == Tapete)
				nextState4 = START1;

		break;
		
		case VALID1 :
			if(SV1 != 0)
				p_SV1 = SV1;
			if ((p_SV1 == 1 && p_SV2 != 1) && (SPR1 == 1 && SPR2 == 1))
				nextState4 = BLUE1;

			else if (p_SV1 == 4 && (SPR1 == 1 && SPR2 == 1))
				nextState4 = GREEN1;

		break;
		
		/*
		00	t1	t4
		01	t4	t2	pe2
		04	t4	t3
		10	t1	t2
		11	t4	t2 	pe2
		14	ttodos
		40	t1	t3	pr1
		41	t1	t3	pr1
		44	t1	t3	pr1
		*/

		case BLUE1 :
			if (fe_STR1 == 1)
				nextState4 = PROC1;

		break;
		
		case PROC1 :
			if (fe_ST2 == 1){

				p_SV1 = 0;
				nextState4 = START1;
			}

		break;

		case GREEN1 :
			if (fe_STR1 == 1)
				nextState4 = STRET1;
				
		break;
			
		case STRET1 :
			if (SPE1 == 1)
				nextState4 = RETR1;

		break;
			
		case RETR1 :
			if (SPR1 == 1)
				nextState4 = FIN1;

		break;
		
		case FIN1 :
			if (fe_ST3 == 1){

				p_SV1 = 0;
				nextState4 = START1;
			}

		break;
			
	}
}

void Tapetes_2(){
	// sm controls tapetes
	switch(currentState5) {
		case START2 : 
			if (currentState1 == Operar)
				nextState5 = OP2;

		break;
		
		case OP2 :
			if (SV2 != 0)
				nextState5 = VALID2;
				
			else if (currentState1 == Tapete)
				nextState5 = START2;

		break;
		
		case VALID2 :
			if(SV2 != 0)
				p_SV2 = SV2;

			if (p_SV2 == 4 && p_SV1 != 4 && (SPR1 == 1 && SPR2 == 1))
				nextState5 = GREEN2;

			else if (p_SV1 != 4 && p_SV2 == 1 && (SPR1 == 1 && SPR2 == 1))
				nextState5 = BLUE2;

		break;
		
		case GREEN2 :
			if (fe_STR2 == 1)
				nextState5 = PROC2;

		break;
		
		case PROC2 :
			if (fe_ST3 == 1){

				p_SV2 = 0;
				nextState5 = START2;
			}

		break;

		case BLUE2 :
			if (fe_STR2 == 1)
				nextState5 = STRET2;
				
		break;
			
		case STRET2 :
			if (SPE2 == 1)
				nextState5 = RETR2;

		break;
			
		case RETR2 :
			if (SPR2 == 1)
				nextState5 = FIN2;

		break;
		
		case FIN2 :
			if (fe_ST2 == 1){

				p_SV2 = 0;
				nextState5 = START2;
			}

		break;
				
	}
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
	
		// Leitura das entradas
		read_inputs();

		// Edge detection
		edges();

		// State machines
		operacao();
		wait();
		contador();		
		Tapetes_1();
		Tapetes_2();

		// State update
        currentState1 = nextState1;
		currentState2 = nextState2;
		currentState3 = nextState3;
		currentState4 = nextState4;
		currentState5 = nextState5;

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