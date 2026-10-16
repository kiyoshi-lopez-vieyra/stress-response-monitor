*Sistema de Monitoreo de **Estrés  ·**  Documentación técnica*

**SISTEMA DE MONITOREO FISIOLÓGICO**

**PARA EVALUACIÓN DE RESPUESTA AL ESTRÉS COGNITIVO**

*Correlación entre frecuencia cardiaca y temperatura periférica*

Documentación técnica del proyecto

Hardware · Firmware · Aplicación · Protocolo experimental

**ESP****32  +****  MAX30102  +  DS18B20**

Resumen ejecutivo

El presente documento describe el desarrollo, implementación y operación de un sistema integral de monitoreo fisiológico no invasivo diseñado para evaluar la respuesta del organismo ante estímulos de estrés cognitivo. El sistema combina hardware embebido, firmware en tiempo real, comunicación inalámbrica y una aplicación web interactiva multiplataforma.

El objetivo central del proyecto es estudiar la correlación entre dos variables fisiológicas mensurables —la frecuencia cardiaca (BPM) y la temperatura periférica corporal— durante la aplicación controlada de pruebas cognitivas estandarizadas en investigación de estrés.

La metodología empleada se basa en protocolos reconocidos en literatura científica (Stroop Test, N-Back, PASAT) e incorpora un análisis estadístico automático mediante el coeficiente de correlación de Pearson, generando datos exportables en formato CSV para análisis posterior.

| **Componentes principales del sistema** 1) Microcontrolador ESP32 con capacidad WiFi.  2) Sensor óptico MAX30102 para frecuencia cardiaca por fotopletismografía.  3) Sensor digital DS18B20 para temperatura periférica.  4) Aplicación web responsiva con comunicación WebSocket en tiempo real.  5) Tres pruebas cognitivas integradas con grabación sincronizada de datos. |
| --- |

1. Introducción y fundamento teórico

1.1 Justificación del proyecto

La respuesta fisiológica al estrés constituye un campo de estudio fundamental en disciplinas como la medicina, la psicología, la fisiología del ejercicio y la ergonomía. Cuando un individuo es sometido a un estímulo estresor —ya sea físico, emocional o cognitivo—, su sistema nervioso autónomo activa una serie de respuestas mediadas principalmente por la rama simpática, las cuales se manifiestan en variables observables como la frecuencia cardiaca, la temperatura corporal, la conductancia de la piel y la presión arterial.

Entre estas variables, la frecuencia cardiaca y la temperatura periférica resultan particularmente interesantes porque pueden medirse de manera no invasiva mediante sensores de bajo costo, y porque presentan una respuesta complementaria al estrés: mientras la frecuencia cardiaca tiende a incrementarse, la temperatura periférica suele disminuir debido a la vasoconstricción simpática.

Este proyecto se propone construir, desde cero, un sistema que permita observar y registrar simultáneamente ambas variables durante la aplicación controlada de pruebas cognitivas estandarizadas, generando datos cuantitativos que permitan caracterizar la respuesta individual al estrés.

1.2 Bases fisiológicas

1.2.1 La respuesta de estrés

El estrés activa el sistema nervioso simpático y el eje hipotálamo-hipófisis-suprarrenal. La activación simpática provoca, en cuestión de segundos, la liberación de catecolaminas (adrenalina y noradrenalina) que producen efectos sistémicos rápidos. Esta respuesta es adaptativa y prepara al organismo para enfrentar la amenaza percibida (respuesta de lucha o huida).

1.2.2 Frecuencia cardiaca y estrés

En condiciones de reposo, la frecuencia cardiaca de un adulto sano oscila típicamente entre 60 y 100 latidos por minuto. La activación simpática durante el estrés aumenta el cronotropismo cardíaco mediante la estimulación del nodo sinoauricular, produciendo un incremento en la frecuencia cardiaca.

Es importante señalar que la frecuencia cardiaca presenta una variabilidad natural latido a latido (HRV, Heart Rate Variability), la cual es considerada un signo de salud cardiovascular. Esta variabilidad implica que dos lecturas consecutivas no sean idénticas; pequeñas fluctuaciones de ±2 a ±5 BPM son fisiológicamente normales y reflejan la modulación constante del sistema nervioso autónomo sobre el corazón.

1.2.3 Temperatura periférica y estrés

La temperatura medida en zonas periféricas (dedos, manos, pies) no equivale a la temperatura corporal central. La temperatura corporal central, medida en cavidades internas como la boca, el recto o la membrana timpánica, oscila normalmente entre 36.5 °C y 37.5 °C. La temperatura periférica, en cambio, puede ser sustancialmente menor —típicamente entre 28 °C y 35 °C— debido a la pérdida de calor al ambiente.

Durante el estrés, la activación simpática provoca vasoconstricción periférica como mecanismo de redistribución sanguínea hacia órganos vitales. Esta vasoconstricción reduce el flujo de sangre caliente hacia las extremidades, causando un descenso medible en la temperatura periférica que puede oscilar entre 0.3 °C y 2 °C dependiendo de la intensidad del estímulo.

| **Hipótesis del estudio** Durante la aplicación de una prueba cognitiva estresante, se espera observar un incremento en la frecuencia cardiaca y un descenso en la temperatura periférica, lo cual debería reflejarse en una correlación negativa entre ambas variables, característica de la respuesta simpática. |
| --- |

2. Arquitectura del hardware

2.1 Microcontrolador ESP32

El sistema utiliza como unidad central de procesamiento un microcontrolador ESP32 de Espressif Systems, seleccionado por sus características técnicas adecuadas para aplicaciones de telemetría inalámbrica de bajo costo.

El ESP32 integra en un solo chip un procesador dual-core a 240 MHz, conectividad WiFi 802.11 b/g/n y Bluetooth, así como múltiples interfaces de comunicación periférica (I2C, SPI, UART, OneWire). Estas características lo hacen ideal para la adquisición simultánea de datos provenientes de varios sensores y su transmisión en tiempo real a través de la red local.

La elección del ESP32 sobre alternativas como Arduino UNO o NodeMCU se justifica por tres razones principales: la capacidad de manejar comunicación WebSocket con la potencia necesaria, la disponibilidad de pines GPIO con soporte nativo para I2C en velocidad rápida, y la facilidad de programación a través del entorno Arduino IDE con las librerías estándar.

2.2 Sensor de frecuencia cardiaca MAX30102

El MAX30102 es un sensor óptico integrado que combina dos diodos LED (uno rojo de 660 nm y uno infrarrojo de 880 nm), un fotodetector, y un convertidor analógico-digital de 18 bits, todos encapsulados en un módulo de 5.6 × 3.3 mm.

Su principio de funcionamiento se basa en la fotopletismografía (PPG). El sensor emite luz infrarroja hacia el tejido (típicamente la yema del dedo) y mide la intensidad de la luz reflejada. Con cada latido cardíaco, el volumen de sangre en los capilares aumenta momentáneamente, provocando una mayor absorción de luz y, por lo tanto, una menor cantidad de luz reflejada hacia el fotodetector. Esta variación cíclica de intensidad lumínica permite identificar cada pulso cardiaco y calcular la frecuencia.

Especificaciones técnicas relevantes

| **Parámetro** | **Valor** |
| --- | --- |
| Voltaje de operación | 1.8 V – 3.3 V |
| Interfaz de comunicación | I2C |
| LED infrarrojo | 880 nm |
| LED rojo | 660 nm |
| Resolución ADC | 18 bits |
| Frecuencia de muestreo | Hasta 3200 muestras/segundo |
| Corriente de operación | ~600 µA típico |

2.3 Sensor de temperatura DS18B20

El DS18B20 es un sensor digital de temperatura fabricado por Maxim Integrated (anteriormente Dallas Semiconductor). Es ampliamente utilizado en aplicaciones biomédicas, industriales y de laboratorio por su precisión, su interfaz simple y su robustez.

Para este proyecto se utiliza la versión en encapsulado TO-92 (cápsula plástica negra de tres terminales), la cual ofrece una respuesta térmica más rápida que la versión de tubo de acero inoxidable, debido a su menor masa térmica. Esta característica es particularmente relevante en aplicaciones donde se requiere detectar cambios sutiles de temperatura en escalas de tiempo cortas, como las que se esperan durante el estrés.

Especificaciones técnicas relevantes

| **Parámetro** | **Valor** |
| --- | --- |
| Voltaje de operación | 3.0 V – 5.5 V |
| Interfaz de comunicación | 1-Wire (OneWire) |
| Rango de medición | -55 °C a +125 °C |
| Precisión | ±0.5 °C en el rango -10 °C a +85 °C |
| Resolución programable | 9 a 12 bits |
| Tiempo de conversión | ~750 ms a 12 bits |

2.4 Diagrama de conexiones

La integración eléctrica del sistema sigue el siguiente esquema de conexiones entre el microcontrolador ESP32 y los sensores:

MAX30102 → ESP32

| **Pin del sensor** | **Pin del ESP32** | **Función** |
| --- | --- | --- |
| VIN | 3.3V | Alimentación |
| GND | GND | Tierra común |
| SDA | GPIO 21 | Línea de datos I2C |
| SCL | GPIO 22 | Línea de reloj I2C |

DS18B20 → ESP32

| **Pin del sensor** | **Pin del ESP32** | **Función** |
| --- | --- | --- |
| VCC | 3.3V | Alimentación |
| GND | GND | Tierra común |
| DATA | GPIO 4 | Línea de datos OneWire |

Adicionalmente, se requiere una resistencia de pull-up de 4.7 kΩ entre las líneas VCC y DATA del DS18B20, necesaria para el correcto funcionamiento del protocolo OneWire. Esta resistencia permite que la línea de datos regrese al estado lógico alto cuando ningún dispositivo está transmitiendo activamente.

| **Consideración de diseño** Ambos sensores operan a 3.3 V, lo cual elimina la necesidad de circuitos de conversión de niveles lógicos y simplifica considerablemente el diseño eléctrico. El consumo total del sistema es lo suficientemente bajo para permitir su alimentación por puerto USB estándar o batería portátil. |
| --- |

3. Firmware del ESP32

3.1 Visión general

El firmware desarrollado para el ESP32 implementa cuatro funcionalidades principales que operan de manera concurrente: la adquisición de datos del sensor MAX30102, la lectura periódica del sensor DS18B20, el establecimiento de la conexión WiFi con el punto de acceso, y la transmisión de datos en tiempo real mediante el protocolo WebSocket.

El programa está escrito en C++ utilizando el entorno Arduino IDE, lo que permite aprovechar el ecosistema de librerías estándar y simplificar el proceso de desarrollo y mantenimiento. El código sigue una estructura modular que facilita la depuración y la extensión futura del sistema.

3.2 Librerías utilizadas

| **Librería** | **Autor** | **Función** |
| --- | --- | --- |
| WiFi.h | Espressif | Conexión WiFi del ESP32 |
| WebSocketsServer | Markus Sattler | Servidor WebSocket |
| ArduinoJson | Benoit Blanchon | Construcción de mensajes JSON |
| Wire | Arduino | Comunicación I2C |
| MAX30105 | SparkFun | Control del MAX30102 |
| heartRate | SparkFun | Algoritmo de detección de pulso |
| OneWire | Paul Stoffregen | Protocolo 1-Wire |
| DallasTemperature | Miles Burton | Interfaz para DS18B20 |

Es importante destacar que la librería MAX30105 de SparkFun funciona correctamente con el sensor MAX30102, ya que ambos sensores comparten el mismo núcleo y conjunto de registros internos. Sólo cambia el encapsulado y la presencia o ausencia del LED verde adicional en el MAX30105.

3.3 Algoritmo de detección de pulso

El proceso de obtención de la frecuencia cardiaca a partir de la señal óptica del MAX30102 consta de varias etapas:

- **Lectura del valor IR: **el sensor envía cada pocos milisegundos una muestra del nivel de luz infrarroja detectada por el fotodiodo.

- **Detección de latido: **la función checkForBeat() de la librería SparkFun identifica los picos en la señal correspondientes a cada latido cardíaco.

- **Cálculo de intervalo: **se calcula el tiempo transcurrido entre latidos consecutivos.

- **Conversión a BPM: **se aplica la fórmula BPM = 60 / (intervalo en segundos) para obtener la frecuencia instantánea.

- **Filtrado por rango: **se descartan valores fuera del rango fisiológico aceptable (40-200 BPM).

- **Suavizado por promedio móvil: **se mantiene un buffer circular de 20 latidos cuyo promedio se calcula y se reporta.

- **Suavizado exponencial: **el valor reportado se acerca gradualmente al promedio, evitando saltos abruptos.

3.4 Detección de presencia del dedo

Para determinar si el usuario está colocando el dedo sobre el sensor, se monitorea el valor IR sin procesar (irValue) proveniente del fotodiodo. Cuando no hay dedo, la luz infrarroja se dispersa libremente y el valor IR es bajo. Cuando se coloca el dedo, este actúa como medio reflectante y absorbente, incrementando significativamente la cantidad de luz que regresa al sensor.

Empíricamente se ha determinado un umbral de 50 000 unidades como criterio para distinguir entre la presencia y la ausencia del dedo. Por debajo de este valor se considera que el dedo no está presente y el sistema reinicia el cálculo de BPM para evitar reportar valores obsoletos.

3.5 Comunicación WebSocket

Para la transmisión de datos del ESP32 hacia la aplicación se utilizó el protocolo WebSocket, el cual ofrece varias ventajas respecto a alternativas como HTTP polling o MQTT:

- Conexión bidireccional y persistente con baja latencia.

- Eficiencia en el uso de ancho de banda al evitar la sobrecarga de cabeceras HTTP en cada mensaje.

- Implementación sencilla tanto en el servidor (ESP32) como en el cliente (navegador web).

- Tiempo real verdadero, sin necesidad de que el cliente solicite repetidamente actualizaciones.

El servidor WebSocket se ejecuta en el ESP32 sobre el puerto 81. Cada 1.5 segundos se envía a todos los clientes conectados un mensaje JSON con la siguiente estructura:

| {   "bpm": 78,   "temp": 34.6,   "finger": true,   "raw_ir": 102450 } |
| --- |

Los campos transmitidos son: la frecuencia cardiaca calculada (bpm), la temperatura medida en grados Celsius (temp), el estado de detección del dedo (finger) y el valor IR sin procesar para diagnóstico (raw_ir).

3.6 Código fuente comentado

A continuación se presenta el código completo del firmware con sus comentarios explicativos:

_(El código completo está en `firmware/stress_monitor/stress_monitor.ino`.)_

_(El código completo está en `firmware/stress_monitor/stress_monitor.ino`.)_

_(El código completo está en `firmware/stress_monitor/stress_monitor.ino`.)_

4. Aplicación web

4.1 Decisiones de diseño

La aplicación cliente fue desarrollada como una página web única (single-page application) utilizando HTML5, CSS3 y JavaScript puro, sin dependencias de frameworks pesados. Esta decisión arquitectónica responde a varios objetivos:

- **Multiplataforma: **al ejecutarse en un navegador web, la aplicación funciona indistintamente en Android, iOS, Windows, macOS o Linux.

- **Sin instalación: **el usuario sólo necesita abrir el archivo HTML en cualquier navegador moderno, sin necesidad de pasar por tiendas de aplicaciones.

- **Operación local: **toda la aplicación funciona en red local entre el ESP32 y el dispositivo del usuario, sin requerir conexión a internet.

- **Bajo consumo: **el código completo pesa menos de 100 kB y no requiere recursos significativos del dispositivo.

La interfaz fue diseñada con un enfoque mobile-first, optimizada para uso en pantallas pequeñas pero adaptable a resoluciones mayores. Se utilizó la librería Chart.js para la generación de gráficas dinámicas en tiempo real.

4.2 Estructura de la aplicación

La aplicación se compone de cuatro pantallas principales que guían al usuario a través del flujo experimental completo:

Pantalla 1 — Configuración

Es el punto de entrada de la aplicación. Aquí el usuario debe:

- Ingresar un identificador del paciente (anónimo) para la trazabilidad de los datos.

- Especificar la dirección IP local del ESP32 y establecer la conexión WebSocket.

- Seleccionar la prueba cognitiva que se aplicará durante la fase de estrés (Stroop, N-Back o PASAT).

- Configurar la duración de cada una de las tres fases del experimento (reposo, prueba y recuperación).

Pantalla 2 — Estabilización de sensores

Una vez que el usuario coloca el dedo sobre los sensores, esta pantalla evalúa en tiempo real la estabilidad de las señales antes de permitir que comience la sesión. Cada sensor cuenta con su propia tarjeta visual que muestra el valor actual, el estado de estabilización y una barra de progreso.

Los criterios de estabilización son los siguientes: para la frecuencia cardiaca, la variación máxima entre 8 muestras consecutivas debe ser menor o igual a 5 BPM; para la temperatura, dicha variación debe ser menor o igual a 0.3 °C. Cuando ambas señales alcanzan estabilidad, se desbloquea automáticamente el botón para iniciar la sesión.

Este mecanismo es importante porque garantiza que los datos basales sean confiables y representativos, evitando que mediciones tomadas durante el periodo de adaptación contaminen el análisis.

Pantalla 3 — Sesión activa

Durante esta pantalla se desarrolla el experimento completo. El usuario ve en tiempo real ambas variables fisiológicas con sus respectivas gráficas, así como un indicador visual de la fase actual y un cronómetro de progreso.

Las tres fases se ejecutan en secuencia: reposo, fase de estrés (con la prueba cognitiva seleccionada) y recuperación. Durante cada fase, la aplicación graba continuamente los datos junto con una etiqueta de la fase activa, generando un registro temporal completo.

Pantalla 4 — Resultados

Al finalizar la sesión, esta pantalla muestra automáticamente:

- Promedios de frecuencia cardiaca por fase (reposo, estrés, recuperación).

- Promedios de temperatura por fase.

- Coeficiente de correlación de Pearson entre BPM y temperatura, con interpretación cualitativa.

- Resultados de la prueba cognitiva (aciertos, errores y precisión porcentual).

- Gráfica combinada de toda la sesión con ambas variables superpuestas.

- Opción de exportar todos los datos en formato CSV para análisis posterior en Excel u otras herramientas.

4.3 Comunicación entre ESP32 y aplicación

La comunicación entre el ESP32 y la aplicación se realiza mediante el protocolo WebSocket sobre la red WiFi local. El ESP32 actúa como servidor y la aplicación como cliente. Una vez establecida la conexión, el ESP32 envía continuamente mensajes JSON cada 1.5 segundos sin necesidad de que la aplicación los solicite.

La aplicación parsea cada mensaje recibido, actualiza la interfaz, alimenta las gráficas en tiempo real y, durante una fase activa, registra los datos en un arreglo interno para su posterior exportación. Si la conexión se pierde por cualquier motivo (apagado del ESP32, movimiento fuera del rango WiFi), la aplicación lo detecta y notifica al usuario con un indicador visual.

5. Pruebas cognitivas integradas

La aplicación incluye tres pruebas cognitivas reconocidas en la literatura científica como inductoras de estrés. Cada una activa procesos cognitivos distintos pero comparte el objetivo común de generar una carga mental que active el sistema nervioso simpático.

5.1 Stroop Test

5.1.1 Origen y fundamento

El Stroop Test fue descrito por primera vez por John Ridley Stroop en 1935 en su tesis doctoral, y desde entonces se ha convertido en uno de los paradigmas más utilizados en neurociencia cognitiva. La prueba aprovecha el conflicto cognitivo conocido como efecto Stroop o interferencia color-palabra.

El efecto consiste en lo siguiente: cuando una persona ve una palabra que representa un color (por ejemplo, la palabra ROJO) pero esa palabra está escrita con tinta de un color diferente (por ejemplo, en azul), el cerebro experimenta una competencia entre dos procesos automáticos: la lectura de la palabra (proceso muy automatizado en lectores experimentados) y el reconocimiento del color. Para responder correctamente al color de la tinta, se debe inhibir la respuesta automática de leer la palabra, lo cual requiere control ejecutivo y atención sostenida.

5.1.2 Implementación en la aplicación

En la versión implementada, se muestran palabras de cuatro colores (ROJO, AZUL, VERDE, AMARILLO) escritas en tintas distintas a la palabra que representan. El paciente debe tocar el botón correspondiente al color de la tinta —no a la palabra— dentro de un tiempo límite de 1.8 segundos por estímulo.

Si el paciente no responde a tiempo, la respuesta se contabiliza como error. Esta restricción temporal incrementa significativamente la carga de estrés respecto a la versión clásica sin límite temporal.

5.2 N-Back

5.2.1 Origen y fundamento

La tarea N-Back fue desarrollada por Wayne Kirchner en 1958 y posteriormente popularizada en estudios de memoria de trabajo. Es una de las pruebas más utilizadas en investigación de neuroimagen funcional para evocar actividad en la corteza prefrontal dorsolateral.

Su fundamento es la memoria de trabajo: la capacidad limitada del cerebro para mantener información activa durante períodos cortos mientras se realizan operaciones mentales sobre ella. La sobrecarga de esta capacidad genera estrés cognitivo cuantificable.

5.2.2 Implementación en la aplicación

La versión integrada es de tipo 1-Back: el paciente ve una secuencia de letras presentadas una a la vez y debe indicar, para cada letra, si es idéntica a la letra mostrada inmediatamente antes. Las respuestas se dan tocando los botones SÍ o NO.

Aproximadamente el 40% de las letras se repiten respecto a la anterior, garantizando un balance adecuado entre estímulos objetivo y distractores. El tiempo límite por estímulo es de 2 segundos.

5.3 PASAT (Paced Auditory Serial Addition Test)

5.3.1 Origen y fundamento

El PASAT fue desarrollado por Gronwall y Sampson en 1974 como una herramienta para evaluar la velocidad de procesamiento de información en pacientes con traumatismo craneoencefálico. Posteriormente se ha utilizado ampliamente en estudios sobre esclerosis múltiple, déficit de atención y como inductor experimental de estrés cognitivo.

La versión clásica requiere que el paciente sume mentalmente cada número que escucha con el número anterior, manteniendo el ritmo impuesto. Esta tarea sobrecarga la atención dividida, la memoria de trabajo y la velocidad de procesamiento simultáneamente.

5.3.2 Implementación en la aplicación

La versión implementada presenta una adaptación visual y simplificada del PASAT: en lugar de sumas auditivas continuas, se muestran operaciones aritméticas (sumas y restas) con cuatro opciones de respuesta posibles, una de las cuales es correcta y las otras tres son distractoras numéricamente cercanas al resultado real.

Los números en las sumas oscilan entre 15-94 y 10-49, mientras que en las restas se utilizan rangos de 30-99 y 5-29. El paciente cuenta con 3.5 segundos por operación, y los timeouts se contabilizan como errores.

| **Comparación de las tres pruebas** Stroop: activa el control inhibitorio y la atención selectiva.  N-Back: sobrecarga la memoria de trabajo activa.  PASAT: combina cálculo mental, velocidad de procesamiento y presión temporal. Los tres protocolos están validados en literatura científica como inductores de estrés cognitivo medible. |
| --- |

6. Protocolo experimental

6.1 Estructura general

Cada sesión experimental sigue un protocolo de tres fases consecutivas, basado en los estándares utilizados en investigación psicofisiológica:

| **Fase** | **Duración sugerida** | **Actividad del sujeto** | **Propósito** |
| --- | --- | --- | --- |
| 1. Reposo | 120 segundos | Quieto, respiración natural | Establecer línea base |
| 2. Estrés cognitivo | 180 segundos | Prueba seleccionada | Inducir activación simpática |
| 3. Recuperación | 120 segundos | Quieto, respiración natural | Observar el retorno a basal |

Las duraciones son configurables por el operador desde la pantalla inicial, lo cual permite adaptar el protocolo a diferentes objetivos de investigación o limitaciones de tiempo.

6.2 Preparación del sujeto

Antes de iniciar la sesión, es importante considerar varios factores que pueden afectar la calidad de los datos obtenidos:

- **Ambiente: **el espacio debe estar a temperatura ambiente estable (idealmente 22-25 °C) y libre de corrientes de aire que afecten la temperatura periférica.

- **Estado fisiológico: **se recomienda que el sujeto no haya consumido cafeína u otros estimulantes en las dos horas previas, ni haya realizado actividad física intensa.

- **Postura: **el sujeto debe estar sentado cómodamente con el brazo apoyado, manteniendo la mano relajada y en una posición que no comprima los sensores.

- **Colocación de sensores: **el dedo índice debe descansar sobre el MAX30102 sin presionar excesivamente. La presión excesiva comprime los capilares y degrada la señal.

6.3 Procedimiento operativo

- **Paso 1: **Encender el ESP32 y esperar a que se establezca la conexión WiFi. Anotar la dirección IP asignada.

- **Paso 2: **Abrir la aplicación en el dispositivo móvil o computadora conectado a la misma red.

- **Paso 3: **Ingresar el identificador del sujeto, la dirección IP y conectar al WebSocket.

- **Paso 4: **Seleccionar la prueba cognitiva y configurar las duraciones de fase.

- **Paso 5: **Colocar los sensores al sujeto y esperar la estabilización automática de ambas señales.

- **Paso 6: **Iniciar la sesión y seguir el protocolo de tres fases.

- **Paso 7: **Al finalizar, revisar los resultados automáticos y exportar el archivo CSV para análisis posterior.

7. Análisis de datos

7.1 Estructura del archivo CSV

Cada sesión genera un archivo CSV con la siguiente estructura de columnas:

| **Columna** | **Tipo** | **Descripción** |
| --- | --- | --- |
| timestamp | ISO 8601 | Marca temporal absoluta del registro |
| elapsed_s | Decimal | Segundos transcurridos desde el inicio |
| fase | Texto | reposo / estres / recuperacion |
| bpm | Entero | Frecuencia cardiaca en latidos por minuto |
| temperatura | Decimal | Temperatura periférica en °C |
| test_correcto | 0 / 1 / vacío | Resultado de la respuesta (sólo durante estrés) |

7.2 Coeficiente de correlación de Pearson

La aplicación calcula automáticamente el coeficiente de correlación lineal de Pearson (r) entre los valores de BPM y temperatura registrados a lo largo de toda la sesión. Este coeficiente es una medida estadística que cuantifica la fuerza y dirección de la relación lineal entre dos variables continuas.

El valor de r puede oscilar entre -1 y +1, donde:

- r = +1 indica una correlación lineal positiva perfecta.

- r = 0 indica ausencia de correlación lineal.

- r = -1 indica una correlación lineal negativa perfecta.

La interpretación cualitativa que la aplicación proporciona automáticamente sigue el siguiente esquema:

| **Valor absoluto de r** | **Interpretación** |
| --- | --- |
| 0.00 – 0.39 | Correlación débil o no significativa |
| 0.40 – 0.69 | Correlación moderada |
| 0.70 – 1.00 | Correlación fuerte |

En este estudio, la hipótesis fisiológica predice una correlación negativa (r negativo) entre BPM y temperatura periférica durante la fase de estrés, debido a la activación simpática que incrementa la frecuencia cardiaca y simultáneamente disminuye la temperatura por vasoconstricción.

7.3 Consideraciones interpretativas

Al interpretar los resultados es importante tener presente que:

- Una correlación débil no necesariamente implica que no haya respuesta al estrés. Puede deberse a tiempos de respuesta desfasados entre ambas variables, ya que el BPM responde en segundos mientras que la temperatura puede tardar minutos en cambiar.

- La respuesta individual al estrés varía significativamente entre sujetos. Algunos individuos muestran respuestas marcadas, otros muestran respuestas atenuadas, especialmente si la prueba les resulta familiar o si han desarrollado tolerancia al estímulo.

- Para un análisis más profundo, se recomienda calcular la correlación separada por fases en el archivo CSV exportado, así como graficar la evolución temporal de ambas variables.

8. Limitaciones y consideraciones técnicas

8.1 Sobre la medición de frecuencia cardiaca

El método de fotopletismografía empleado por el MAX30102 es una técnica de medición indirecta. Aunque su precisión es adecuada para la gran mayoría de aplicaciones no clínicas, presenta limitaciones inherentes:

- **Sensibilidad al movimiento: **movimientos del dedo durante la medición introducen artefactos que pueden interpretarse como latidos falsos.

- **Influencia de la presión: **presionar excesivamente sobre el sensor comprime los capilares y reduce la calidad de la señal.

- **Luz ambiental: **luz solar directa o iluminación muy intensa puede interferir con la lectura óptica.

- **Variabilidad natural (HRV): **el corazón no late con un intervalo exactamente constante. Variaciones de ±2 a ±5 BPM entre lecturas consecutivas son normales y reflejan la salud del sistema autónomo.

8.2 Sobre la medición de temperatura

El DS18B20, aunque preciso desde el punto de vista electrónico, presenta consideraciones físicas importantes al medir temperatura corporal:

- **Temperatura periférica vs central: **la temperatura del dedo es naturalmente menor que la temperatura corporal central. Valores entre 28 °C y 35 °C son fisiológicamente normales y no indican mal funcionamiento del sistema.

- **Inercia térmica: **el sensor requiere un tiempo de adaptación al contacto con el cuerpo. Aunque la versión TO-92 utilizada tiene mejor respuesta que el encapsulado de acero, sigue requiriendo entre uno y tres minutos para alcanzar equilibrio térmico.

- **Pérdida de calor al ambiente: **si la cara opuesta del sensor está expuesta al aire, parte del calor recibido del dedo se disipa al ambiente, impidiendo que el sensor alcance la temperatura real del dedo. El aislamiento térmico mejora notablemente la medición.

- **Calidad del contacto físico: **burbujas de aire entre el sensor y la piel actúan como aislantes térmicos. El contacto debe ser firme pero no compresivo.

8.3 Sobre la respuesta al estrés

La inducción experimental de estrés mediante pruebas cognitivas presenta variabilidad considerable entre individuos. Factores que pueden modular la magnitud de la respuesta incluyen:

- Habituación: la repetición de la misma prueba reduce su capacidad de inducir estrés en el mismo sujeto.

- Diferencias en la activación basal del sistema nervioso autónomo entre individuos.

- Estado emocional del sujeto al momento de la sesión.

- Familiaridad con el tipo de tarea cognitiva utilizada.

- Componente social o evaluativo (las pruebas con observador externo suelen generar mayor respuesta de estrés).

| **Importancia del análisis relativo** Para los objetivos de este proyecto, lo relevante no son los valores absolutos de cada variable sino los cambios relativos entre las fases. Una caída de 0.5 °C de la temperatura entre el reposo y el estrés, o un aumento de 8 BPM en la frecuencia cardiaca, son datos significativos independientemente de que los valores absolutos sean menores que los esperados. |
| --- |

9. Conclusiones y posibles extensiones

9.1 Logros del proyecto

El sistema desarrollado cumple los objetivos planteados al inicio del proyecto: permite registrar simultáneamente y en tiempo real la frecuencia cardiaca y la temperatura periférica de un sujeto durante la aplicación de pruebas cognitivas estandarizadas, generando datos cuantitativos exportables para análisis estadístico posterior.

El sistema integra de manera coherente todos los componentes necesarios para un experimento psicofisiológico básico: adquisición de señales biológicas, control temporal de fases experimentales, presentación controlada de estímulos cognitivos, registro sincronizado de respuestas conductuales y fisiológicas, y análisis estadístico automático.

Adicionalmente, el costo total del hardware utilizado es accesible (típicamente menor a 300 pesos mexicanos), lo cual lo hace replicable en contextos educativos y de investigación con recursos limitados.

9.2 Posibles extensiones futuras

Varias mejoras y ampliaciones podrían incorporarse al sistema en futuras versiones:

- **Medición de SpO2: **el MAX30102 también soporta el cálculo de saturación de oxígeno en sangre mediante el uso simultáneo del LED rojo e infrarrojo, lo cual añadiría una variable fisiológica adicional.

- **Variabilidad de la frecuencia cardiaca (HRV): **calcular métricas avanzadas como RMSSD o pNN50, las cuales son indicadores sensibles del estrés.

- **Almacenamiento en la nube: **integración con una base de datos remota para acumular sesiones de múltiples sujetos y realizar análisis poblacionales.

- **Pruebas adicionales: **incorporar Go/No-Go, Trail Making u otros paradigmas neuropsicológicos.

- **Estresores físicos: **incluir el Cold Pressor Test (introducción de la mano contralateral en agua fría) como estresor fisiológico complementario.

- **Sensor de conductancia de la piel: **añadir un módulo GSR para registrar la respuesta electrodérmica, otra variable autónoma sensible al estrés.

9.3 Reflexión final

Más allá de los resultados específicos que pueda producir cada sesión experimental, el principal valor de este proyecto reside en haber construido un instrumento funcional y replicable que integra principios de electrónica, programación embebida, desarrollo web, psicofisiología y análisis estadístico. El proceso de desarrollo ha implicado decisiones de ingeniería en cada nivel del sistema, desde la selección de los sensores apropiados hasta el diseño de algoritmos de filtrado de señal, pasando por el diseño de la experiencia de usuario en la aplicación.

El sistema queda como una plataforma base sobre la cual pueden construirse experimentos más sofisticados, ampliarse las variables medidas, o adaptarse a contextos de aplicación específicos en docencia, investigación o evaluación clínica.

Página 1 de 2