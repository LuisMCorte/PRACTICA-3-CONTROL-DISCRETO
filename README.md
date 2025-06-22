# PRACTICA-3-CONTROL-DISCRETO

Esta parte delinforme describe el funcionamiento de un sistema implementado en la placa Controllino para
controlar dos LEDs mediante modulación por ancho de pulso (PWM). El sistema utiliza dos botones para
alternar el estado de los LEDs (encendido/apagado) y una interfaz hombre-maquina (HMI) para ajustar el
ciclo de trabajo de la señal PWM. La implementaci´on emplea una máquina de estados finitos (FSM) basada
en enums, técnicas de antirrebote para los botones y temporización no bloqueante con millis(). El código
desarrollado se incluye como anexo.


Funcionamiento General del Sistema
El sistema controla dos LEDs conectados a los pines CONTROLLINO_D0 y CONTROLLINO_D1 de la placa
Controllino, utilizando modulacion por ancho de pulso (PWM) para ajustar su brillo. Dos botones, conectados
a CONTROLLINO_I16 y CONTROLLINO_I17, permiten alternar el estado de cada LED entre encendido (ON) y
apagado (OFF). Una interfaz hombre-maquina (HMI) proporciona valores de ciclo de trabajo (0 % a 100 %)
para cada LED, que se convierten a valores PWM (0 a 255) mediante la funcion map().
La logica se implementa mediante una maquina de estados finitos (FSM) definida por un enum LedState
con dos estados: OFF y ON. Cada LED tiene su propio estado (stateD0 y stateD1), actualizado al detectar
una pulsación válida en los botones. Para evitar falsos positivos por rebote mecanico, se utiliza una tecnica
de antirrebote basada en millis() con un retardo de 30 ms (Delay). Cuando un LED esta en ON y el ciclo
de trabajo es válido (0 % a 100 %), se aplica el valor PWM correspondiente; de lo contrario, el LED se apaga
(PWM = 0).
La comunicacion serial a 115200 baudios permite monitorear los estados y ciclos de trabajo de los LEDs en
tiempo real. La inicializacion de la HMI, mediante las funciones HMI_init() y Stone_HMI_Set_Value(), establece los valores iniciales de los ciclos de trabajo en 0 %. El sistema es eficiente y no bloqueante, garantizando
una respuesta inmediata a las entradas de los botones y la HMI.
