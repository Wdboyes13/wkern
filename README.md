# WKern  
**This is a kernel/OS I'm working on**  
**For Current Lines GO TO [loc.md](/loc.md)**  
__The specs of the machine are very important__  
__Since I dont have the energy to port it to working on other machines__  
  
__(E-Mail william@mail.weelam.ca if you want me to build it for you)__
__THIS WILL ONLY WORK IN THE PRESET QEMU VM CONFIG RUN WITH `make test`__  

## Tools
__Make sure to run the script to check these with `./configure.sh`__  
__For Platform-Specific Info on installing these go to [docs/platform/](/docs/platform/) And Select your OS__   
__For the new list of requirements, check [RequiredTools.txt](/RequiredTools.txt)__  

## To Run

```sh
git clone --depth 1 https://github.com/Wdboyes13/slre src/slre # Do to fun times with licensing, SLRE Regex needs to be cloned separatly
./build.py -mkd   # This creates the disk that the OS will use
./build.py        # This will run ALL Compilation and Tests
./build.py -run   # This will run QEMU Using the required config
```

## This project is licensed under the GNU General Public License v3
## For more info, see [`LICENSE`](/LICENSE)
## Or for the File Header [`FileHeader`](/FileHeader)