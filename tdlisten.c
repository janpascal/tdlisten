#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <telldus-core.h>

int done=0;

int verbose=0;
int continuous=0;
int device=-1;

void WINAPI handler(int deviceId, int method, const char *data, int callbackId, void *context) {
    if(verbose) {
        printf("deviceId %d, method %d\n", deviceId, method);
        printf("data: \"%s\"\n", data);
        printf("callbackId %d\n", callbackId);
    } else {
        if(deviceId==device) {
            printf("%d", method);
        }
    }
    if(!continuous) done=1;
}

int find_device(char *device) {
    int deviceId = atoi(device);
    if (deviceId == 0) { //Try to find the id from the name
        int intNum = tdGetNumberOfDevices();
        int index = 0;
        while (index < intNum) {
            int id = tdGetDeviceId(index);
            char *name = tdGetName( id );
            if (strcasecmp(name, device) == 0) {
                deviceId = id;
                tdReleaseString(name);
                break;
            }
            tdReleaseString(name);
            index++;
        }
    }
    return deviceId;
}

void usage() {
    fprintf(stderr, "Usage: tdlisten [-c] [-v] [-d device]\n");
    fprintf(stderr, "  -c\t\tcontinous (do not exit when event received)\n");
    fprintf(stderr, "  -v\t\tverbose\n");
    fprintf(stderr, "  -d device\tselect device by name or id\n");
    fprintf(stderr, "  -h\t\tshow this help text\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Wait for events from the given telldus device and\n");
    fprintf(stderr, "print the new device state (TELLSTICK_TURNON, etc)\n");
    fprintf(stderr, "Device state codes:\n");
    fprintf(stderr, "\tTELLSTICK_TURNON        1\n");
    fprintf(stderr, "\tTELLSTICK_TURNOFF       2\n");
    fprintf(stderr, "\tTELLSTICK_BELL          4\n");
    fprintf(stderr, "\tTELLSTICK_TOGGLE        8\n");
    fprintf(stderr, "\tTELLSTICK_DIM           16\n");
    fprintf(stderr, "\tTELLSTICK_LEARN         32\n");
    fprintf(stderr, "\tTELLSTICK_EXECUTE       64\n");
    fprintf(stderr, "\tTELLSTICK_UP            128\n");
    fprintf(stderr, "\tTELLSTICK_DOWN          256\n");
    fprintf(stderr, "\tTELLSTICK_STOP          512\n");
}

void handle_options(int argc, char* argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "cd:hv")) != -1) {
        switch (opt) {
        case 'v':
            verbose++;
            break;
        case 'c':
            continuous=1;
            break;
        case 'd':
            device=find_device(optarg);
            if(device==0) {
                fprintf(stderr, "Device \"%s\" not found\n", optarg);
                exit(EXIT_FAILURE);
            }
            break;
        case 'h':
            usage();
            exit(EXIT_SUCCESS);
        default: /* '?' */
            usage();
            exit(EXIT_FAILURE);
        }
    }

}

int main(int argc, char* argv[]) {
    tdInit();
    handle_options(argc, argv);

    if(verbose) {
        printf("Waiting for events from device %d\n", device);
    }

    int callbackId = tdRegisterDeviceEvent(handler, NULL);

    // Wait for event
    for(;;) {
        usleep(10000);
        if(done) break;
    }

    tdUnregisterCallback(callbackId);
    tdClose();
    return 0;
}
