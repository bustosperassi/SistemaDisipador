import sys
import serial
import pygtk
pygtk.require('2.0')
import gtk 
import time
import thread
import threading
gtk.gdk.threads_init()

class TempSysApp:
	label = gtk.Label("00")
	label2 = gtk.Label("00")
	ser=serial.Serial('/dev/ttyUSB0', 9600)
	
	def funcion_boton(self, widget):
		
		if widget.get_active():
			self.ser.write('1')
			print "Cooler Prendido"
		else:
			self.ser.write('0')
			print "Cooler Apagado"
	
	def worker(self):
		while True:			
			self.ser.write('2')
			time.sleep(0.1)
			temp = hex(int(ord(self.ser.read())))
			temp= int(temp,0)
			#print "TempSen %s\n" % temp
			self.label.set_text(str(temp))
			time.sleep(0.1)
			self.ser.write('3')
			time.sleep(0.1)
			temp = hex(int(ord(self.ser.read())))
			temp= int(temp,0)
			#print "TempSet %s\n" % temp
			self.label2.set_text(str(temp))
			time.sleep(0.4)	
	

	def __init__(self):
			
		#=============Ventana Principal=================					
		window = gtk.Window(gtk.WINDOW_TOPLEVEL)
		window.set_title("Disipador de Temperatura")
		window.connect("destroy", lambda x: gtk.main_quit())
		window.set_border_width(10)
		main_vbox = gtk.VBox(False, 0)
		window.add(main_vbox)
		
		#=============Frame Temp Sensada=================
		frame_acciones = gtk.Frame("Temperatura Sensada")
		main_vbox.pack_start(frame_acciones,  False,  False, 10)
		acciones_hbox = gtk.HBox(False, 0)
		acciones_hbox.set_border_width(10)
		frame_acciones.add(acciones_hbox)
		
		#==============Etiqueta===========================
		acciones_hbox.add(self.label)
		label1 = gtk.Label("C")
		acciones_hbox.add(label1)
				
		#=============Frame Temp Seteada=================
		frame_acciones2 = gtk.Frame("Temperatura Seteada")
		main_vbox.pack_start(frame_acciones2,  False,  False, 10)
		acciones2_hbox = gtk.HBox(False, 0)
		acciones2_hbox.set_border_width(10)
		frame_acciones2.add(acciones2_hbox)
		
		#==============Etiqueta===========================
		acciones2_hbox.add(self.label2)
		label3 = gtk.Label("C")
		acciones2_hbox.add(label3)
		
		#=============Frame Boton=================
		frame_acciones3 = gtk.Frame("")
		main_vbox.pack_start(frame_acciones3,  False,  False, 10)
		acciones3_hbox = gtk.HBox(False, 0)
		acciones3_hbox.set_border_width(10)
		frame_acciones3.add(acciones3_hbox)
	
		#========Boton Ventilador==========================
		boton1=gtk.ToggleButton("Prender Ventilador")
		boton1.connect("toggled", self.funcion_boton)
		acciones3_hbox.pack_start(boton1, False, False,10)
		
		
		wTemp = threading.Thread(target=self.worker)
		wTemp.start()
		self.label.show()
		label1.show()
		self.label2.show()
		label3.show()
		window.show_all()
		
		
				
def main():
	gtk.main()
	return 0

if __name__ == "__main__":
    TempSysApp()
    main()
