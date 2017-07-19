def env_setup(d,s) :
    #import commands   #python3 does not working
    import subprocess

    #commands.getoutput('touch %s/source_dir_path.txt' % bb.data.getVar("BASE_WORKDIR", d, True))  #python3 does not working
    subprocess.getoutput('touch %s/SOURCE_PATH_FOR_OPTEE.txt' % d.getVar("BASE_WORKDIR", True))
    binary_location_file_path = d.getVar("BASE_WORKDIR", True) + "/SOURCE_PATH_FOR_OPTEE.txt"
    with open(binary_location_file_path, "rt") as file :
        for line in file :
            if s in line :
                return line.strip()
            else :
                pass

    return "not-exist-in-recipes"

def env_setup_kernel(d,s) :
    #import commands    #python3 does not working
    import subprocess

    #commands.getoutput('touch %s/source_kernel_dir_path.txt' % bb.data.getVar("BASE_WORKDIR", d, True))   #python3 does not working
    subprocess.getoutput('touch %s/KBUILD_OUTPUT_PATH.txt' % d.getVar("BASE_WORKDIR", True))
    binary_location_file_path = d.getVar("BASE_WORKDIR", True) + "/KBUILD_OUTPUT_PATH.txt"
    with open(binary_location_file_path, "rt") as file :
        for line in file :
            if s in line :
                return line.strip()
            else :
                pass

    return "not-exist-in-recipes"
