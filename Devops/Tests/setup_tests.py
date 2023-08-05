import glob
import os
import shutil
import subprocess
from distutils.dir_util import copy_tree

test_directory = r'E:\MyUnrealProjects\TurnBasedRPGCombat\Devops\Tests'
test_data_directory = rf'{test_directory}\Data'

ue_automation_content_path = r'E:\UE\UE_5.1\Engine\Content\Automation'

def remove_directory(directory_to_remove):
    if os.path.exists(directory_to_remove):
        shutil.rmtree(directory_to_remove)
    else:
        print('no ' + directory_to_remove + ' directory exists to remove')

def copy_directory(directory_to_copy, destination):
    if os.path.exists(directory_to_copy):
        copy_tree(directory_to_copy, destination)
    else:
        print('no ' + directory_to_copy + ' directory exists to copy')


remove_directory(test_data_directory)

copy_directory(ue_automation_content_path, test_data_directory)

subprocess.call('npm install -g bower', shell=True)

os.chdir(test_data_directory)
subprocess.call(fr'bower install {test_data_directory}\bower.json', shell=True)
os.chdir(test_directory)

subprocess.call('npm install http-server bower -g', shell=True)