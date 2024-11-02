#include <iostream>
#include <string>
#include "Logger.cpp"
#include "Mount_class.cpp"

using namespace std;

void setup_loop_device(const char* loopDevice, const char* imageFile) {

    char command[256];
    snprintf(command, sizeof(command), "losetup %s %s", loopDevice, imageFile);

    // Выполняем команду
    int result = system(command);
    if (result == -1) {
        cerr << "Error executing losetup: " << strerror(errno) << endl;
    } else {
        cout << "Successfully set up " << imageFile << " on " << loopDevice << endl;
    }
}

int main()
{
    Logger l = Logger("log.csv");

    MountLinux mountLinux(l);
    const char* m = "/mnt/img";
    const char* i = "/mnt/ext4_image.img";
    const char* n = "/dev/loop0";

    //setup_loop_device(n, i);

    mountLinux.umount_ext(m, i, n);
    //mountLinux.mount_ext(m, i, n);

}