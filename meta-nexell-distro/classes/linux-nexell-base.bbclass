inherit linux-kernel-base

def get_kernel_arch(d, a):
    if 'aarch64' in a :
        return 'arm64'
    else:
        return 'arm32'

def get_kernel_arch_num(d, a):
    if 'aarch64' in a :
        return '64'
    else :
        return '32'

#a1 ==> Distinguish string, DEPLOY_DIR
#a2 ==> Validate feed string, MACHINE_ARCH
def get_image_type(d, a1, a2):
    dist = []
    dist = a1.split('/')
    validate = a2.split('_')
    for i in dist:
        cnt = 0
        for j in validate:
            if j in i :
                cnt += 1

        #board soc name & board prefix & board postfix matched!
        if cnt == 3:
            if 'sato' in i :
                return 'sato'
            elif 'qt' in i :
                return 'qt'
            elif 'genivi' in i :
                return 'genivi'
            elif 'agl' in i :
                return 'agl'
            elif 'tinyui' in i :
                return 'tinyui'
            elif 'sdl' in i :
                return 'sdl'
            elif 'smartvoice' in i :
                return 'smartvoice'
            else :
                return 'tiny'

def get_kernel_image_type(d, s):
    board_soc_name = s.split('-')[0]

    if board_soc_name == 's5p4418' :
        return 'zImage'
    elif board_soc_name == 's5p6818' :
        return 'Image'
    else :
        return 'Not supported board SOC!!'

def get_defaulttune_type(d, s):
    board_soc_name = s.split('-')[0]

    if board_soc_name == 's5p4418' :
        return 'cortexa9hf-neon'
    elif board_soc_name == 's5p6818' :
        return 'aarch64'
    else :
        return 'Not supported board SOC!!'
