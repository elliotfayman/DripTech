# DripTech


This program is designed for a smart water drip sensor that uses advanced signal processing to distinguish dripping noise from background noise, helping to identify leaks and conserve water. The program runs on an Arduino microcontroller and employs various techniques to isolate and identify dripping noise.

## Program Functionality

The program starts by collecting a rolling average of the raw microphone audio, producing a smoother signal for further processing. A bandpass filter is then applied to attenuate high and low frequency noise that does not originate from a dripping faucet in a sink. This narrows down the frequency range considered when identifying dripping sounds.

To further isolate the signal, the program calculates a running average of the entire signal, excluding any outliers that could significantly skew the average. This value is then subtracted from all noise inputs, resulting in a value that represents the amount of noise above the average. This step removes additional background noise that could interfere with the ability to accurately locate each drip noise.

A cutoff noise amplitude is used to determine whether a sound is potentially the actual dripping noise of the sink. The program records the time of each detected signal and continues to do so until it has captured three distinct times. If the time intervals between the first and second point and the second and third point are similar, it confirms that periodic noise is present. If multiple instances of periodic noise are detected, it can conclude that dripping noise is present.

## Additional Features

The smart water sensor also includes a buzzer to alert household members when dripping water is detected, and an LCD screen that displays the duration of the water leak in seconds and the volume of water wasted in milliliters. This dual-screen feature allows users to easily monitor both the duration and quantity of the water leak, providing valuable insights to help them take action and prevent further waste.

## Electric Schematic

## Author

Elliot Fayman, Shilpa Bojjireddy, Rajashekar Vennavelli, and Morgan Barrett.

