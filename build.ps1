param (
    [switch][alias('h')]$help = $true,
    [switch][alias('v')]$visualstudio = $false,
    [switch][alias('d')]$development = $false,
    [switch][alias('s')]$shipping = $false,
    [string][alias('a')]$alpakit,
    [switch][alias('anc')]$alpanocopy = $false, 
    [string][alias('m')]$modref, 
    [string][alias('u')]$unrealdir = "$PSScriptRoot\Unreal Engine - CSS",
    [string][alias('sml')]$smldir = "$PSScriptRoot\SatisfactoryModLoader"
 )


$smldir = $smldir+"\FactoryGame.uproject"

function show-help(){
    Write-Host " Powershell script for running Satisfactory Mod Building options from Powershell in Windows.
    -h | help - This help dialog.
    -v | visualstudio - Generate Visual Studio Project files
    -d | development - Build for Development Editor
    -s | shipping - Build for Shipping
    -a | alpakit - Run Alpakit. Takes an argument of the game directory to copy to or NoCopy to bypass e.g. `-a=`"C:SteamLib\Satisfactory\FactoryGame.Exe`"' or '-a=NoCopy'
    -m | modref - Required if using alpakit - the Reference name of your mod e.g. '-m=MyAwesomePlugin'
    -u | unrealdir - Specify a path to '\Unreal Engine - CSS' - defaults to expecting that directory to be in the same location as the script e.g '-u=`"C:Modding\Unreal Engine - CSS`"'
    -sml | smldir - Specify a path to the '\SatisfactoryModLoader' - defaults to expecting that directory to be in the same location as the script e.g. '-sml=`"C:Modding\SatisfactoryModLoader`"'

    Can specify more than one - will run them in the same order they are described in the above help, so you can: '& build.ps1 -v -d -s -a=NoCopy -p=MyPlugin'
    "
}

try{


    if ($visualstudio) {
        $help = $false
        $unrealbuildtool = $unrealdir+"\Engine\Binaries\DotNET\UnrealBuildTool.exe"
        Invoke-Expression ".`"$unrealbuildtool`" -projectfiles -project=`"$smldir`" -game -rocket -progress"
    }

    if ($development) {
        $help = $false
        $unrealbuildbat = $unrealdir+"\Engine\Build\BatchFiles\Build.bat"
        Invoke-Expression ".`"$unrealbuildbat`" FactoryGameEditor Win64 Development -project=`"$smldir`" -WaitMutex -FromMsBuild"
    }

    if ($shipping) {
        $help = $false
        $unrealbuildbat = $unrealdir+"\Engine\Build\BatchFiles\Build.bat"
        Invoke-Expression ".`"$unrealbuildbat`" FactoryGameEditor Win64 Shipping -project=`"$smldir`" -WaitMutex -FromMsBuild"
    }

    if ($alpakit){
        $help = $false
        if (!$modref) {
            Write-Output "No Mod Reference specified for alpakit. Please use -m=ModReferenceName"
        } else {
            $alpakitbat = $unrealdir+"\Engine\Build\BatchFiles\RunUAT.bat"
            alpakit_copy = ""
            if ($alpakit.ToLower() -ne "nocopy")  {
                if ((Test-Path -Path $alpakit)) {
                    alpakit_copy = "-CopyToGameDir -GameDir=`"{$alpakit}`""
                } else {
                    Write-Output "Could not find Satisfactory Game at path $alpakit"
                }
            }
            Invoke-Expression ".`"$alpakitbat`" -ScriptsForProject=`"$smldir`" -Project = `"$smldir`" -PluginName=`"$modref`" $alpakit_copy"
        }
        
    }

    if ($help) {
        show-help
        <# Action to perform if the condition is true #>
    }


    return
}
catch {

    Write-Host "The following eror occured:
    #################################
    
    $_

    #################################

    Please make sure the following values are correct:
        - Unreal Engine-CSS Build Tool path : $unrealdir
        - Satisfactory Mod Loader FactoryGame.uproject path: $smldir"
    if ($alpakit) {
        Write-Host "- [Alpakit] Satisfactory Game Factory Location: $alpakit
        - [Alpakit] Mod Reference name: $modref"
    }

    Write-Host "
    If all the above values are correct, then the fault most likely lies outside this simple script.
    "
}


