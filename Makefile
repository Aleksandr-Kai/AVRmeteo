default:
	avrdude -c usbasp -p atmega16 -U flash:w:Debug/Exe/meteo.hex
read:
	avrdude -c usbasp -p atmega16 -U flash:r:Debug/Exe/rmeteo.hex
