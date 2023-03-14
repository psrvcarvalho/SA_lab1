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
	GREE,
	BLUE,
} stateNames3;

// "Tapetes" state machine
typedef enum{
	// TBD
} stateNames4;

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
stateNames4 currentState4 = //TBD

// cycle time
uint64_t  scan_time = 1000;	// 1sec
/* uint64_t  stoptime = 10000; // 10sec */

// Falling edges
bool fe_START = False, fe_STOP = False, fe_ST2 = False, fe_ST3 = False;

// Previous values
bool p_START = 0,  p_STOP = 0, p_ST2 = 0, p_ST3 = 0;

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

	// updates values
	p_START = START;
	p_STOP = STOP;
	p_ST2 = ST2;
	p_ST3 = ST3;
	
}

void operação(){
	// sm controls operação
	switch (currentState1) {
			case Parado :
					
				// Transition Parado -> Operar
				if (fe_START == 1)
					// Next State
					currentState1= Operar;
			
				break;
			
			case Operar :
				
				// Transition Operar -> A_Parar
				if (fe_STOP == 1)
					// Next State
					currentState1= A_Parar;
	
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
					currentState1= Tapete;
					
				break;

			case Tapete :
				
				// Transition Tapete -> Parado
				if (timer1.time >= 15000)
					// Next State
					currentState1= Parado;
					
				break;
	} //end case

}

void wait(){
	// sm controls wait
	switch (currentState2) {
			case LOFF :
				// Transition LOFF -> LON
				if ((currentState1 == A_Parar) && timer2.time >= 2000)
					currentState2 == LON;

				break

			case LON :
				// Transition LON -> LOFF
				currentState2 == LOFF;

			break


}

void contador(){
	// sm controls contador
}

void tapetes(){
	// sm controls tapetes
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
		operação();
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
		
		//Escrita nas saídas
		write_outputs();

		
		//Aguarda pelo próximo ciclo
		sleep_abs(scan_time);
		
	} // end loop
	
} // end main