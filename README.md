# Microprocessors Project

## Project Goal

The purpose of this project is to explore and demonstrate how we can control the rotation direction of a servo motor using **Pulse Width Modulation (PWM)** signals, based on data provided by a **flame sensor**.

The project focuses on developing a system capable of interpreting **analog signals** from the flame sensor in order to generate corresponding **PWM signals** for controlling the servo motor's position.

A vital aspect of the project is its ability to transmit sensor data to a computer via the **UART (Universal Asynchronous Receiver-Transmitter)** interface, allowing real-time data visualization through a **graphical interface**. This functionality not only enhances the system’s interactivity but also provides a platform for **data analysis and monitoring**.

## Bonus Functionality

We extended the project by including an additional feature that allows the servo motor position to be controlled using the **TSI (Touch Sensing Input)** sensor integrated into the microcontroller.

## Running the Python Interface

To run the Python interface, follow these steps:

1. Follow the instructions in the `README` file located in the `GUI+env` folder to install dependencies and set up the Python environment.
2. Open PowerShell in the `GUI + env` folder.
3. Run the following command to activate the environment:
   ```powershell
   . .\venv-up-all\Scripts\activate

  #### 4. Run command "python ./main.py".
