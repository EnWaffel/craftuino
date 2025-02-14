# Dokumentation (DE)

ALLE Befehle müssen mit einem '/' anfangen!

[Befehls Liste](#befehls-liste)  
[Funktions Headers](#funktions-headers)

## Befehls Liste

- [/delay](#delay)
- [/execute](#execute)
- [/function](#function)
- [/getpin](#getpin)
- [/gvar](#gvar)
- [/pinmode](#pinmode)
- [/serial](#serial)
- [/set](#set)
- [/setpin](#setpin)
- [/var](#var)

## Befehls Beschreibung

### /delay

Verzögert die Ausführung.

> Benutzung:
> /delay [time (int, var)] ['ms', 'u', 's' (string)]

Beispiel:

```none
/delay 500 ms
```

### /var

Erstellt eine (lokale) Variable und initialisiert sie mit einem Wert.

> Benutzung:
> /var [name (string)] [? (literal)]

Beispiel:

```none
/var meineVariable 100
```

### /gvar

Erstellt eine (globale) Variable und initialisiert sie mit einem Wert.

> Benutzung:
> /gvar [name (string)] [? (literal)]

Beispiel:

```none
/gvar meineGlobaleVariable 100
```

### /function

Ruft eine Funktion auf.

> Benutzung:
> /function [functionName (string)] [... (literals, vars)]

Beispiel:

```none
/function meineFunktion meinArgument1 meinArgument2 meinArgument3
```

### /getpin

Speichert den Wert von einem Pin in einer Variable.

> Benutzung:
> /getpin [pin (int, var)] [variable (var)] \(optional\)['DIGITAL', 'ANALOG' (option)]

Beispiel:

```none
/getpin 5 meineVariable         <--- Das selbe, wie
/getpin 5 meineVariable DIGITAL <--- das hier.
/getpin A0 meineVariable ANALOG
```

### /setpin

Setzt den Wert vom einem Pin.

> Benutzung:
> /setpin [pin (int, var)] ['HIGH', 'LOW' (option)]

Beispiel:

```none
/setpin 5 HIGH
/setpin 5 LOW
```

### /set

Setzt den Wert von einer Variable.

> Benutzung:
> /set [name (string)] [? (literal, var)]

Beispiel:

```none
/set meineVariable 99
/set meineVariable meineAndereVariable
```

### /pinmode

Setzt den Modus von einem Pin.

> Benutzung:
> /pinmode [pin (int, var)] ['INPUT', 'OUTPUT' (option)]

Beispiel:

```none
/pinmode 5 OUTPUT
/pinmode A0 INPUT
```

### /serial

Serielle Kommunikation (mit dem Standard Seriellen Ausgang).

> Benutzung:  
> /serial <begin, print, println>  
> /serial begin [baudRate (int)]  
> /serial print [message (any)]  
> /serial println [message (any)]

Beispiel:

```none
/serial begin 9600
/serial print "Mein Wert: "
/serial println meineWertVariable
```

### /execute

Führe z.B. If-Checks durch.

> Benutzung:  
> /execute if [? (literal, var)] ['=', '|', '&' (option)] [? (literal, var)]

Beispiel:

```none
/execute if meineVariable = 100 run /serial println "Wert ist 100!"
```

## Funktions Headers

Funktions Headers beschreiben eine Funktion und können extra Informationen oder andere Preprocess Befehle beinhalten.

### Funktion Header Liste

- '-rt' | Gibt den Rückgabewert einer Funktion an. (Für einfache Funktionen 'void' benutzen)
- '-p' | Gibt Parameter für die Funktion an. (Datentyp:ParameterName;Datentyp:ParameterName2 usw...)
- '-cos' | Gibt an, welche Funktionen beim Start ausgeführt werden sollen. (Bevor der Code von 'setup' ausgeführt wird)
