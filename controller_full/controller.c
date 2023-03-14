// ---------- Máquina de estados -----------

#include "IO.c"


#undef DEBUG

// Tipos de dados

// Estados da máquina
typedef enum{
	Parado,
	Operar,
	A_Parar,
	Tapete,
} stateNames;

// Timers
typedef struct {
    uint32_t start_time,
    uint32_t timeout,
    bool active,
} Timer;




// Funções
void initME();

// Estado atual da máquina
stateNames currentState = Parado;

// Tempo de ciclo
uint64_t  scan_time = 1000;	// 1sec
uint64_t  stoptime = 10000; // 10sec

// Falling edges
bool fe_START = False, fe_STOP = False, fe_ST2 = False, fe_ST3 = False;

// Previous values
bool p_START = 0,  p_STOP = 0, p_ST2 = 0, p_ST3 = 0;

void edges(){
	//Detects edges

	//FALLING EDGES
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

	//updates values
	p_START = START;
	p_STOP = STOP;
	p_ST2 = ST2;
	p_ST3 = ST3;
	
}

void operação(){
	//sm controls operação
	switch (currentState) {
			case Parado :
					
				// Transition Parado -> Operar
				if (fe_START == 1)
					// Next State
					currentState = Operar;
			
				break;
			
			case Operar :
				
				// Transition Operar -> A_Parar
				if (fe_STOP == 1)
					// Next State
					currentState = A_Parar;
	
				break;

			case A_Parar :
				
				// Initialize the timer to 10 seconds
    			sm->timer.start_time = millis();
    			sm->timer.timeout = 10000;
    			sm->timer.active = true;


				// Transition A_Parar -> Tapete
				if (LOW == 0)
					// Next State
					currentState = A_Parar;
					
				break;

			case Tapete :
				
				// Transition Tapete -> Parado
				if (LOW == 0)
					// Next State
					currentState = A_Parar;
					
				break;
	} //end case

}

void wait(){
	//sm controls wait
}

void contador(){
	// sm controls contador
}

void tapetes(){
	// sm controls tapetes
}

// Inicializa a ME
void init_ME()
{
	//LSTOP = 1;
}

// Código principal
int main() {

	
	// Inicialização da ME
	init_ME();
	
	// Ciclo de execução
	while(1) {

		#ifdef DEBUG
		printf ("\n*** Inicio do Ciclo ***\n");
		#endif

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

		// Transição entre estados
		
		
		

		// Atualiza saídas

		// Saídas booleanas
		LSTOP = (currentState == Parado);
		LSTART = (currentState == Operar);
		E1 = (currentState == Operar);
		E2 = (currentState == Operar);
		T2A = (currentState == Tapete);
		T3A = (currentState == Tapete);
		
		//Escrita nas saídas
		write_outputs();

		//timer1
		start_timer(&timer1);

		if(timer1.time >= 10000)

		stop_timer(&timer1);

		//Aguarda pelo próximo ciclo
		sleep_abs(scan_time);
		
	} // end loop
	
} // end main
