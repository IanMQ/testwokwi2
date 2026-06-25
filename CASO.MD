

INGENIERÍA DE SOFTWARE
1Asi0572 – DESARROLLO DE SOLUCIONES IOT
PRÁCTICA CALIFICADA 2
202610

NRC: 		6770
Profesor:	Prudencio Vidal, Javier Antonio
		
Duración:	100 minutos
Indicaciones:

    1. El examen consta de 2 preguntas en base a un caso, y tendrá 100 minutos para resolverlas.
    2. Las preguntas son en relación a un Caso y la entrega de su respuesta es a través de envío de dos archivos: un archivo .zip, conteniendo la exportación de su solución a la pregunta desarrollo de software y el documento de PowerPoint upc-pre-202610-1asi0572-pc2-file_v1.pptx con su respuesta para la pregunta de redacción, colocando el contenido en la hoja de respuesta según el título e indicaciones. 





Enunciado:
Caso Libelium
Libelium (https://www.libelium.com/) es una compañía pionera y referente mundial en el Internet of Things (IoT), proveedora de hardware y plataformas que conectan el mundo físico con las redes de comunicación, impulsando el desarrollo de ciudades inteligentes (Smart Cities) sostenibles en todo el planeta.
Libelium ofrece a las administraciones municipales y a los integradores una plataforma de gestión en la nube, con accesos en:
Web Console: https://www.libelium.com/iot-products/
Documentación técnica: https://development.libelium.com/
Libelium Cloud es la plataforma que permite el control y monitoreo de los dispositivos inteligentes desplegados en la ciudad. Al conectar un dispositivo con Libelium Cloud, la plataforma se convierte en el panel de control y visualización en tiempo real, todo en uno.
Para los Clientes ofrece también el portal https://www.libelium.com/contact/, así como https://development.libelium.com/, donde permite registrar sus productos, hacer seguimiento a órdenes y brinda información sobre integraciones.
Actualmente desea introducir Smart Parking Spot S1, un nuevo miembro en su línea Libelium Smart Cities.
Features:
El dispositivo basado en ESP32 se instala en una plaza de estacionamiento público en vía pública e incluye: un Ultrasound Distance Sensor ubicado sobre la plaza, apuntando hacia abajo, para detectar si hay un vehículo ocupando el espacio (Occupancy Detector). Cuando la plaza está libre el sensor marca 300 cm (distancia al pavimento); se considera que un vehículo está ocupando la plaza cuando la distancia medida es de 50 cm o menos. Cuenta también con un PIR Motion Sensor para detectar movimiento durante una maniobra de estacionamiento. El dispositivo incluye un Servo Motor que acciona una barrera/bolardo retráctil que se eleva para reservar la plaza cuando esta ha sido reservada de forma remota desde la app, e impedir su ocupación no autorizada. Además cuenta con un RGB LED indicador de estado: verde cuando la plaza está libre, rojo cuando está ocupada y azul cuando está reservada. También cuenta con un Digital display 16x2 I2C. El display muestra el estado de la plaza (“FREE”, “BUSY” o “RESERVED”) y el tiempo de ocupación transcurrido en minutos (en Min). La tarifa máxima de estacionamiento permitida es de 120 minutos. El operador recomienda emitir una alerta cuando se supera el 80% del tiempo permitido. Si la plaza está ocupada (Occupancy Detector ≤ 50 cm) y el tiempo de ocupación supera los 96 minutos (80% del máximo), se debe encender de forma intermitente el RGB LED en rojo y mostrar “OVERTIME” en el display hasta que el vehículo abandone la plaza. Cuando la plaza es reservada de forma remota, el Servo Motor eleva el bolardo y el RGB LED se enciende en azul. En este modo, la plaza no admite la detección de ocupación por otro vehículo, hasta que la reserva sea cancelada o el vehículo autorizado se aproxime (PIR Motion Sensor detecta movimiento). El dispositivo cuenta además con información de tracking de operación para el Device Maker.
Technical constraints:
    • El prototipo debe elaborarse con Wokwi y la programación debe ser con Arduino sketch (C++) aplicando Object-Oriented Programming y basado en ModestIoT para el Embedded Application, creando clases y objetos para representar el estado y comportamiento de los diversos tipos de sensors y actuators en el device.
    • El contenido de sketch.ino debería enfocarse en la generación de instancia de un objeto de tipo Device e invocar al comportamiento correspondiente en los métodos setup() y loop() (no debería haber lógica de comportamiento en loop, solo la instrucción para inhibir el loop).
    • Aplique convenciones de nomenclatura en inglés, con principios, patrones y buenas prácticas de diseño de software orientado a objetos.
    • Declare constantes para evitar los magic string y magic number anti-patterns.
    • Documente con comentarios en inglés sus archivos de código fuente con las convenciones de Doxygen (ver Doxygen: Documenting the code en referencias).
    • Al inicio de la ejecución debe mostrarse en consola la información de la empresa, así como sus nombres y apellidos como developer miembro del equipo de Libelium. Considere en sus objetos y archivo sketch el código necesario para describir y demostrar los modos de operación y actualización de estado requeridos en el IoT Device.
    • Exporte el proyecto de Wokwi y empaquete el mismo como un archivo .zip. (único formato válido) con el nombre pc2<NRC>-u<código-estudiante>.zip (por ejemplo, pc26779u201621873.zip).
    • Suba dos archivos: El archivo upc-pre-202610-1asi0572-pc2-file_v1.pptx con sus respuestas (renombrado como pc2<NRC>u<código-estudiante>-file.pptx, por ejemplo pc26779u201621873-file.pptx, sin empaquetar) y el archivo .zip con el proyecto de solución de Wokwi, en la Actividad indicada para la Práctica calificada.
En Wokwi, el archivo libraries.txt debe contener:
# Wokwi Library List
# See https://docs.wokwi.com/guides/libraries
ArduinoJson
PubSubClient
ESP32Servo
LiquidCrystal I2C
NewPing
Adafruit Unified Sensor
EspSoftwareSerial
ModestIoT@wokwi:fb939e3848beed149e380d7392ca7967aa440c62

En base al caso:
Pregunta 1 (5 p.).
Bajo el marco de los 12 pasos del IoT System Design Steps (ver Anexo A), elabore el Definition of application service layer requirements (requisitos relacionados con la capa de servicio de la aplicación). Aquí considere a) Definición de la interfaz de usuario para cada servicio prestado (dependiendo del servicio la aplicación para dicho servicio podría requerir una interfaz gráfica para móvil, o tablet o desktop o más de un tipo).
Pregunta 2 (15 p.)
Cree un nuevo proyecto en Wokwi y desarrolle un prototipo que satisfaga los requisitos planteados para el Smart Parking Spot S1.
Referencias

Wokwi Parts:
https://docs.wokwi.com/guides/esp32/
https://docs.wokwi.com/parts/wokwi-hc-sr04
https://docs.wokwi.com/parts/wokwi-pir-motion-sensor
https://docs.wokwi.com/parts/wokwi-servo
https://docs.wokwi.com/parts/wokwi-lcd1602
https://docs.wokwi.com/parts/wokwi-rgb-led
Wokwi reference projects:
https://wokwi.com/projects/424318978955716609
https://wokwi.com/projects/407174213564042241
Smart Parking references:
https://lastminuteengineers.com/arduino-sr04-ultrasonic-sensor-tutorial/
https://randomnerdtutorials.com/esp32-pir-motion-sensor-interrupts-timers/
Doxygen: Documenting the code
https://www.doxygen.nl/manual/docblocks.html


 
Rúbrica de calificación

Criterio de Calificación	Sobresaliente (S)	Esperado (E)	Necesita Mejorar (M)	Insuficiente (I)	Calificación
C01. Definition of application service layer requirements	Define de forma clara y completa los requisitos relacionados con la capa de servicio de la aplicación, en términos de definición de la interfaz de usuario para cada servicio prestado, estando en todos los casos alineados con las necesidades de la solución de IoT.	Define de forma clara y completa la mayoría de los requisitos relacionados con la capa de servicio de la aplicación, en términos de definición de la interfaz de usuario para cada servicio prestado, estando en todos los casos alineados con las necesidades de la solución de IoT.	Define algunos de los requisitos relacionados con la capa de servicio de la aplicación, en términos de definición de la interfaz de usuario para cada servicio prestado, o en varios casos estos no están alineados con las necesidades de la solución de IoT.	No brinda respuesta o lo especificado no tiene relación con el caso.	
	5.0 puntos	3.5 puntos	1.5 punto	0 puntos	
C02. IoT Device Design	El diseño de prototipo de dispositivo cumple de forma completa con la configuración solicitada. 	El diseño del prototipo de dispositivo cumple con la mayoría de los requisitos sobre configuración.	El diseño del prototipo de dispositivo cumple con algunos de los requisitos sobre configuración.	No diseña prototipo de dispositivo o este no cumple con la mayoría de requisitos sobre configuración.	
	5.0 puntos	3.5 punto	1.5 puntos	0 puntos	
C03. Software Features	Se evidencia la funcionalidad de las operaciones solicitadas, proporcionando en cada caso los valores esperados y respondiendo adecuadamente ante las excepciones. 	Se evidencia parcialmente la funcionalidad de las operaciones solicitadas, proporcionando en algunos casos los valores esperados y respondiendo de forma parcialmente adecuada ante las excepciones.	La ejecución no cumple con las funcionalidades solicitadas.	No implementa comportamiento para el dispositivo.	
	5.0 puntos	3.5 punto	1.5 puntos	0 puntos	
C04. Quality Attributes	Utiliza el lenguaje de programación Arduino Sketch C++. En todos los casos Incluye información de propósito y autoría como comentarios. La codificación tiene un estilo claro, indentando los bloques de código según los estándares de programación correspondientes al lenguaje, aplicando una lógica consistente en los métodos, condicionales sin escenarios no contemplados, uso adecuado de reutilización de código para evitar redundancia. Aplica patrones de arquitectura y patrones de diseño. Cumple de forma completa con los technical constraints.	Utiliza el lenguaje de programación Arduino Sketch C++. En la mayoría de los casos Incluye información de propósito y autoría como comentarios. La codificación es funcional, aplica en la mayoría de casos los estándares de indentación de bloques de código, ó existen algunas ineficiencias en la codificación: redundancia ó inconsistencias en la lógica de programación. Aplica parcialmente patrones de arquitectura y patrones de diseño. Cumple con la mayoría de technical constraints.	Utiliza el lenguaje de programación Arduino Sketch C++. En algunos casos Incluye información de propósito y autoría como comentarios. La codificación es funcional, pero solo aplica algunos de los estándares de indentación de bloques de código, ó existen muchas ineficiencias en la codificación: redundancia ó inconsistencias en la lógica de programación. Aplica algunos patrones de arquitectura y patrones de diseño. Cumple con solo algunos de los technical constraints.	No utiliza el lenguaje de programación Arduino Sketch C++, ó la codificación es funcional  pero no se evidencia aplicación de estándares ó criterios de eficiencia en la codificación, con ausencia de comentarios, ó no aplica patrones de arquitectura ni patrones de diseño, o la codificación no es funcional.	
	3.0 puntos	2.0 punto	1.0 puntos	0 puntos	
C05. Naming standards	El desarrollador aplica en todos los nombres de objetos de programación y base de datos como paquetes, componentes, interfaces, clases, objetos, variables, constantes, métodos, la nomenclatura en inglés y la nomenclatura estándar para identificadores de clases, objetos, miembros de programación, así como los recursos.	El desarrollador aplica en la mayoría de casos la nomenclatura en inglés y la nomenclatura estándar para identificadores de clases, objetos, miembros de programación, así como los recursos.	El desarrollador aplica en muy pocos casos la nomenclatura en inglés y la nomenclatura estándar para identificadores de clases, objetos, miembros de programación, así como los recursos. 	El desarrollador no aplica nomenclatura en inglés para los objetos de programación ó recursos.	
	2.0 puntos	1.0 punto	0.5 puntos	0 puntos	
Total	20 puntos	13.5 puntos	7.0 puntos	0 puntos	


Lima, 19 de noviembre del 2025

Anexo A. IoT System Design Steps

Como se trató en clase, los ingenieros Eulalia Balestrieri, Luca De Vito, Francesco Lamonaca, Francesco Picariello, Sergio Rapuano y Ioan Tudosa de la Universidad de Sannio en Italia, proponen un conjunto de design guidelines para IoT System Design, sintetizados en 12 steps.




Anexo B. Edge-To-Cloud Architecture Layers

Este diagrama que ilustra la relación entre las capas Edge Layer, Fog Layer y Cloud layer.



Anexo C. Parámetros de configuración de MQ2 para diversos tipos de gas

//Set math model to calculate the PPM concentration and the value of constants
  MQ2.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ2.setA(574.25); MQ2.setB(-2.222); // Configure the equation to calculate LPG concentration
  /*
    Exponential regression:
    Gas    | a      | b
    H2     | 987.99 | -2.162
    LPG    | 574.25 | -2.222
    CO     | 36974  | -3.109
    Alcohol| 3616.1 | -2.675
    Propane| 658.71 | -2.168
  */