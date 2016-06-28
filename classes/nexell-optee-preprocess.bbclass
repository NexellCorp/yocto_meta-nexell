def env_setup(d,s) :
    import commands

    commands.getoutput('touch %s/source_dir_path.txt' % bb.data.getVar("BASE_WORKDIR", d, True))
    binary_location_file_path = bb.data.getVar("BASE_WORKDIR", d, True) + "/source_dir_path.txt"
    with open(binary_location_file_path, "rt") as file :
        for line in file :
            if s in line :
                return line.strip()
            else :
                pass

    return "not-exist-in-recipes"