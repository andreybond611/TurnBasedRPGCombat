import glob
import os
import shutil
import subprocess
from distutils.dir_util import copy_tree
import webbrowser

unreal_editor_path = r'E:\UE\UE_5.1\Engine\Binaries\Win64\UnrealEditor.exe'
project_path = r'E:\MyUnrealProjects\TurnBasedRPGCombat'
project_name = r'TurnBasedRPGCombat'
run_UAT_path = r'E:\UE\UE_5.1\Engine\Build\BatchFiles\RunUAT.bat'
tests_name = 'TurnBasedRPGCombat'
test_log_path = fr'{project_path}\Build\Tests\Test.log'
uproject_path = project_path + '\\' + project_name + '.uproject'
test_directory = rf'{project_path}\Devops\Tests'
test_data_directory = rf'{test_directory}\Data'

platform = 'Win64'
configuration = 'Development'
build_path = rf'{project_path}\Build'
report_output_path = rf'{project_path}\Build\Tests'

UAT_flags = ['BuildCookRun', fr'-project="{uproject_path}"', fr'-platform={platform}',
              fr'-clientconfig={configuration}', '-build', '-cook']

unreal_editor_flags = [f'-ExecCmds=\"Automation RunTests {tests_name};Quit\"',
 '-log', fr'-abslog="{test_log_path}"', '-nosplash', '-nullRHI', rf'-ReportOutputPath="{report_output_path}"']

def build_project_for_tests(run_UAT_path, UAT_flags):
    subprocess.call([run_UAT_path] + UAT_flags)

def copy_directory(directory_to_copy, destination):
    if os.path.exists(directory_to_copy):
        copy_tree(directory_to_copy, destination)
    else:
        print('no ' + directory_to_copy + ' directory exists to copy')

build_project_for_tests(run_UAT_path, UAT_flags)

# open unreal project

def show_test_report():
    copy_directory(test_data_directory, report_output_path)
    port = 8081
    os.chdir(report_output_path)
    webbrowser.open(f'http://localhost:{port}')
    subprocess.call(f'http-server -p={port}', shell=True)
    os.chdir(test_directory)
    

def run_tests():
    command = fr'{unreal_editor_path} {uproject_path} '
    command += " ".join(unreal_editor_flags)

    subprocess.call(command, shell=True)

run_tests()

show_test_report()
