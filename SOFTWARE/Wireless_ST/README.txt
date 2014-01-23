PROTOCOLO DE COMUNICACIÓN ENTRE ARDUINOS

La cruceta actúa como coordinador. Cuando detecta que debe haber un cambio en las posiciones de los paneles solares, les ordena moverse y ellos obedecen.
Los posibles movimientos que puede tener un panel son:
	-Fijar el servo horizontal en H grados
	-Fijar el servo vertical en V grados

H y V van de 0 a 180 asi que es posible enviar este número como un caracter ASCII e interpretarlo como un número.
El protocolo de comunicación será como sigue:

Cruceta envía V o H según quiera controlar el servo vertical u horizontal del panel
Panel responde 0 si recibe el mensaje con éxito.
Cruceta envía caracter ASCII cuyo valor sea equivalente al ángulo a fijar en el servo
Panel vuelve a responder 0.

En el caso de que la cruceta detecte que un panel no responde 0, reintenta enviar el mensaje un número de veces. Si se pierde, indicarlo de alguna forma. Los reintentos se hacen cada cierta cantidad de segundos.