#include <iostream>
#include <string>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <linux/loop.h> 

using namespace std;

class MountLinux
{
public:
    MountLinux(Logger& l) : logger(l)
    {

    }

    void umount_ext(const char* mountPoint, const char* imageFile,  const char* loopDevice)
    {
        if (umount(mountPoint) == -1) {

            logger.log(Logger::Level::ERR, "Error unmounting: " + *mountPoint  + *strerror(errno));
            return;
        }
    }

    void mount_ext(const char* mountPoint, const char* imageFile,  const char* loopDevice) 
    {


        if (mkdir(mountPoint, 0755) && errno != EEXIST) {
            logger.log(Logger::Level::ERR, "Error creating mount point: " + *strerror(errno));
        }


        int loopFd = open("/dev/loop0", O_RDWR); 

        if (loopFd < 0) {
            logger.log(Logger::Level::ERR, "Error opening loop device: " + *strerror(errno));
        }

        // Открытие образа
         int imgFd = open(imageFile, O_RDONLY);
        if (imgFd < 0) {
            logger.log(Logger::Level::ERR, "Error opening image file: " + *strerror(errno));
            close(loopFd);
            return;
        }

        char command[256];
        snprintf(command, sizeof(command), "losetup %s %s", loopDevice, imageFile);

        // Связывание устройста с образом
        int result = system(command);
        if (result == -1) {
            logger.log(Logger::Level::ERR, "Error executing losetup: " + *strerror(errno));
        } else {
            cout << "Successfully set up " << imageFile << " on " << loopDevice << endl;
        }

        //  Монтирование
        if (mount("/dev/loop0", mountPoint, "ext4", 0, nullptr) == -1) {
            logger.log(Logger::Level::ERR, "Error mounting image: " + *strerror(errno));
        }

        close(imgFd);
        ioctl(loopFd, LOOP_CLR_FD); 
        close(loopFd);

    }


private:
    Logger& logger;

};



