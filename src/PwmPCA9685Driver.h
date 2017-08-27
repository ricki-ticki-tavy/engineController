
#ifndef PWMPCA9685DRIVER_H_
#define PWMPCA9685DRIVER_H_


namespace tankController{

class PwmPCA9685Driver {
  private:
    int readTwoBytes();
    void writeTwoBytes(int data);

    char addr;
  public:
    PwmPCA9685Driver(char addr);
    
    unsigned char setChannelValue(unsigned char channel, int value);
    unsigned char setPwmFrequency(int frequency);
};

}

#endif // PWMPCA9685DRIVER_H_
