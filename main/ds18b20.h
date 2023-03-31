namespace ds18b20 {
    typedef enum {
        OK = 0,
        CRC_ERROR = 1
    } err_t;

    void init();
    err_t read(float *result);
}