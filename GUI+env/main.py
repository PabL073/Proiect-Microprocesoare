import sys
import serial
import queue
import threading
import time
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QPushButton, QLabel, QPlainTextEdit, QGroupBox
from PyQt5.QtGui import QIcon, QFont
from PyQt5.QtCore import QTimer, Qt
import pyqtgraph as pg

class MainWindow(QMainWindow):
    promotie = "2023-2024"
    team = ["Avrămescu Răzvan", "Cioloca Paul"]
    switch_button_state = False

    def __init__(self):
        super().__init__()
        self.setWindowTitle(f"Proiect Microprocesoare {self.promotie}")
        self.setWindowIcon(QIcon("./icon.png"))

       
        self.GUI_flag = True
        self.ser = serial.Serial(port="COM3", baudrate=9600, timeout=1)
        self.data_queue = queue.Queue()
        self.x_data = []
        self.y_data = []
        self.bar_items = []

        self.main_widget = QWidget(self)
        self.setCentralWidget(self.main_widget)
        main_layout = QVBoxLayout(self.main_widget)

        team_box = QGroupBox("Membrii echipei")
        bold_font = QFont()
        bold_font.setBold(True)
        team_box.setFont(bold_font)

        first_member = QLabel(f"Membru 1: {self.team[0]}")
        second_member = QLabel(f"Membru 2: {self.team[1]}")
        team_box_layout = QVBoxLayout()
        team_box_layout.addWidget(first_member, 1)
        team_box_layout.addWidget(second_member, 1)
        team_box.setLayout(team_box_layout)
        main_layout.addWidget(team_box)

        # Horizontal layout for buttons
        button_layout = QHBoxLayout()
        
        vertical_spacer_top = QVBoxLayout()
        vertical_spacer_top.addStretch(1)


        self.start_button = QPushButton("Start", self)
        self.start_button.setStyleSheet("background-color: green; color: white;")
        self.start_button.clicked.connect(self.start_transmission)
        button_layout.addWidget(self.start_button)
        self.start_button.setMaximumWidth(175)
        self.start_button.setMaximumHeight(75)

        self.switch_button = QPushButton("Normal", self)
        self.switch_button.setStyleSheet("background-color: gray; color: white;")
        self.switch_button.clicked.connect(self.toggle_switch)
        button_layout.addWidget(self.switch_button)
        self.switch_button.setMaximumWidth(175)
        self.switch_button.setMaximumHeight(75)

        self.stop_button = QPushButton("Stop", self)
        self.stop_button.setStyleSheet("background-color: red; color: white;")
        self.stop_button.setMaximumWidth(175)
        self.stop_button.setMaximumHeight(75)
        button_layout.addWidget(self.stop_button)
    
        #make the button layout smaller vertically
        vertical_spacer_bottom = QVBoxLayout()
        main_layout.addLayout(vertical_spacer_bottom)
        
        self.stop_button.clicked.connect(self.stop_transmission)
       

        # Add the button layout to the main layout
        main_layout.addLayout(button_layout)

        # Add a stretch after the button layout

        self.text_edit = QPlainTextEdit(self)
        self.text_edit.setReadOnly(True)
        main_layout.addWidget(self.text_edit)
        self.text_edit.setMaximumHeight(100)

        self.alert_message = QLabel("environment state", self)
        self.alert_message.setAlignment(Qt.AlignCenter)
        font = QFont()
        font.setPointSize(12)
        self.alert_message.setFont(font)
        self.alert_message.setMaximumHeight(50)
        main_layout.addWidget(self.alert_message)

        self.start_time = time.time()
        self.plot_widget = pg.GraphicsLayoutWidget(self)
        self.plot = self.plot_widget.addPlot()
        self.plot.setTitle("Sensor Data")
        self.data_line = self.plot.plot([], [])
        self.plot.setYRange(0, 4)
        self.plot.getAxis('left').setTickSpacing(0.5, 0.5)
        main_layout.addWidget(self.plot_widget)

        self.bar_plot_widget = pg.GraphicsLayoutWidget(self)
        self.bar_plot = self.bar_plot_widget.addPlot()
        self.bar_plot.setTitle("Bar Plot")
        self.bar_item = pg.BarGraphItem(x=[1], height=[0], width=0.3)
        self.bar_plot.addItem(self.bar_item)
        main_layout.addWidget(self.bar_plot_widget)

        self.thread = threading.Thread(target=self.read_serial_data, daemon=True)
        self.thread.start()

        self.timer = QTimer()
        self.timer.timeout.connect(self.update_gui)
        self.timer.start(1000)

        self.setGeometry(0, 0, 1500, 1000)
        
        
    def toggle_switch(self):
        self.switch_button_state = not self.switch_button_state
        if self.switch_button_state:
                self.switch_button.setText("Normal")
                self.switch_button.setStyleSheet("background-color: blue; color: white;")
                self.change_led_sequence()
                # Perform actions for the "On" state
        else:
                self.switch_button.setText("Reverse")
                self.switch_button.setStyleSheet("background-color: gray; color: white;")
                # Perform actions for the "Off" state
                self.change_led_sequence_reverse()



    def read_serial_data(self):
        while True:
            
            if self.ser.is_open:
                data = self.ser.readline().decode('utf-8').rstrip()
                
                self.data_queue.put(data)
                self.ser.flushInput()
                time.sleep(0.1)

                
                
    def update_bar_plot(self, x, height):
        # Update the bar plot with new data
        self.bar_item.setOpts(x=x, height=height)
                
    def update_gui(self):
        if not self.data_queue.empty() and self.GUI_flag is True:
            #solve error could not convert string to float: '3.30V'
      
            voltage_value = 0.0
            
            data = self.data_queue.get_nowait()
            self.text_edit.setPlainText(data)  # Overwrite data in the text widget   
            
            try:
                # data is now 3.30V
                
                voltage_str = data.split(' ')[0].rstrip('V')
                if(voltage_str != ''):
                    voltage_value = float(voltage_str)
                # voltage_str = data.split(' ')[2].rstrip('V')
                # voltage_value = float(voltage_str)
                
                if voltage_value > 3.0: 
                    #add above the plot
                    self.alert_message.setText("ALERT!") 
                    self.alert_message.setStyleSheet("background-color: black; color: red;")
                    #change only current bar color
                    color = 'r'
                
                elif voltage_value < 1.0:
                    self.alert_message.setText("Environment is safe") 
                    self.alert_message.setStyleSheet("background-color: green; color: white;")
                    color = 'g'
                    
                elif voltage_value < 2.0 and voltage_value > 1.0:
                    self.alert_message.setText("Warning! Environment is not safe!") 
                    self.alert_message.setStyleSheet("background-color: red; color: white;")
                    color = 'y'
                    
                elif voltage_value < 3.0 and voltage_value > 2.0:
                    self.alert_message.setText ("Alarm! Environment is on fire!") 
                    self.alert_message.setStyleSheet("background-color: red; color: white;")
                    color = 'r'
                    
                
                # Update the plot with new data
                new_bar_item = pg.BarGraphItem(x=[len(self.bar_items) + 1], height=[voltage_value], width=0.3, brush=color)
                self.bar_plot.addItem(new_bar_item)
                self.bar_items.append(new_bar_item)

                
                       
                current_time = time.time() - self.start_time

                self.x_data.append(current_time)
                self.y_data.append(voltage_value)

                self.data_line.setData(self.x_data, self.y_data, pen='g', symbol='o')
         
             
            except ValueError as e:
                print(f"Error parsing data: {e}")
                
                
    def start_transmission(self):
        """Method to handle start transmission."""
        self.send_uart_flag("S")
        # self.start_button.setEnabled(False)
        # self.stop_button.setEnabled(True)
        self.GUI_flag = True
        

    def stop_transmission(self):
        """Method to handle stop transmission."""
        self.send_uart_flag("s")
        # self.start_button.setEnabled(True)
        # self.stop_button.setEnabled(False)
        self.GUI_flag = False
        
    def change_led_sequence(self):
        """Change the LED sequence."""
        self.send_uart_flag("N")
            
    def change_led_sequence_reverse(self):
        """Change the LED sequence."""
        self.send_uart_flag("R")

    def send_uart_flag(self, flag):
        """Send a flag through UART."""
        if self.ser.is_open:
            self.ser.write(flag.encode('utf-8'))
        


def main():
    app = QApplication(sys.argv)
    mainWin = MainWindow()
    mainWin.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()