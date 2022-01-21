<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio Version=4.2.12.129 SP?>
<SwConfiguration CpuAddress="SL1" xmlns="http://br-automation.co.at/AS/SwConfiguration">
  <TaskClass Name="Cyclic#1">
    <Task Name="tk1_in" Source="In_Out.tk1_in.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="timer" Source="process.timer.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="length" Source="process.length.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="clamp" Source="process.clamp.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="carriage" Source="process.carriage.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="blowpin" Source="process.blowpin.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="tk1_out" Source="In_Out.tk1_out.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#2">
    <Task Name="h_pwm" Source="Heating.h_pwm.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="tk2_io" Source="In_Out.tk2_io.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#3">
    <Task Name="extrlift" Source="process.extrlift.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="cutter" Source="process.cutter.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="cutneck" Source="process.cutneck.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="cooldef" Source="process.cooldef.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="blowdef" Source="process.blowdef.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="bdeflash" Source="process.bdeflash.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="tdeflash" Source="process.tdeflash.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="inerpin" Source="process.inerpin.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="highblow" Source="process.highblow.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="lowblow" Source="process.lowblow.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="coolpin" Source="process.coolpin.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="coolpin2" Source="process.coolpin2.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="coolpin3" Source="process.coolpin3.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="submold" Source="process.submold.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="pinpart" Source="process.pinpart.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="screw" Source="process.screw.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="sealer" Source="process.sealer.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="bblowpin" Source="process.bblowpin.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="support" Source="process.support.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="tonnage" Source="process.tonnage.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="robot" Source="process.robot.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="extruder" Source="machine.extruder.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="extru_plk" Source="machine.extru_plk.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="autorun" Source="process.autorun.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#4">
    <Task Name="keyled" Source="screen.keyled.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="manual" Source="machine.manual.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="calib" Source="machine.calib.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="reset" Source="machine.reset.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="workmode" Source="machine.workmode.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="mode" Source="machine.mode.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="panel" Source="screen.panel.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="hmi_cpy" Source="screen.hmi_cpy.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="pump" Source="machine.pump.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="mac_lubr" Source="machine.mac_lubr.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="oil_accu" Source="machine.oil_accu.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="mbusMaster" Source="comm.mbusMaster.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="tdeflashop" Source="process.tdeflashopncls.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#5">
    <Task Name="h_pid" Source="Heating.h_pid.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#6" />
  <TaskClass Name="Cyclic#7" />
  <TaskClass Name="Cyclic#8">
    <Task Name="spc_ctrl" Source="spc.spc_ctrl.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="trend" Source="Heating.trend.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="h_plan" Source="Heating.h_plan.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="scrshot" Source="screen.scrshot.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="hmiset" Source="screen.hmiset.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="password" Source="screen.password.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="modem" Source="comm.modem.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="vpn" Source="screen.vpn.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="recipe" Source="recipeMan.recipe.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="multsave" Source="recipeMan.multsave.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="oncesave" Source="recipeMan.oncesave.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="cfg_cpy" Source="recipeMan.cfg_cpy.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="pvchg" Source="VarChgLog.pvchg.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="alarm" Source="machine.alarm.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="epl" Source="comm.epl.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="energy" Source="machine.energy.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <DataObjects>
    <DataObject Name="mb_cmd" Source="comm.mb_cmd.dob" Memory="UserROM" Language="Simple" />
    <DataObject Name="pv_chg" Source="VarChgLog.pv_chg.dob" Memory="UserROM" Language="Simple" />
    <DataObject Name="var_fix" Source="recipeMan.var_fix.dob" Memory="UserROM" Language="Simple" />
    <DataObject Name="var_mold" Source="recipeMan.var_mold.dob" Memory="UserROM" Language="Simple" />
    <DataObject Name="cfg_dat" Source="recipeMan.cfg_dat.dob" Memory="UserROM" Language="Simple" />
    <DataObject Name="Acp10sys" Source="" Memory="UserROM" Language="Binary" />
  </DataObjects>
  <VcDataObjects>
    <VcDataObject Name="visu15" Source="Visu.visu15.dob" Memory="UserROM" Language="Vc" WarningLevel="2" Compress="false" />
  </VcDataObjects>
  <Binaries>
    <BinaryObject Name="TCData" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="visu1502" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccnum" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpdvnc" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccalarm" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccovl" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcshared" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcfile" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccstr" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccline" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arial" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcctrend" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcchtml" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcgclass" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="tahomabd" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccbtn" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcrt" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccdbox" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="visu1503" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcclbox" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="simsun" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcctext" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccbmp" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcfntttf" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcalarm" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcdsint" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcnet" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="visu1501" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccshape" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcchspot" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arialbd" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vctcal" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcmgr" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcdsloc" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="myxh" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="visvc" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="tahoma" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccurl" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccdt" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccbar" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpfar00" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpdsw" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcxml" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccscale" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcbclass" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="iomap" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="sysconf" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="ashwd" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="asfw" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="User" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arconfig" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="Role" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Acp10map" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="acp10cfg" Source="" Memory="UserROM" Language="Binary" />
  </Binaries>
  <Libraries>
    <LibraryObject Name="visapi" Source="Libraries.visapi.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="FileIO" Source="Libraries.FileIO.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="runtime" Source="Libraries.runtime.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="sys_lib" Source="Libraries.sys_lib.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="standard" Source="Libraries.standard.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="operator" Source="Libraries.operator.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="dvframe" Source="Libraries.dvframe.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="DataObj" Source="Libraries.DataObj.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="Convert" Source="Libraries.Convert.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="brsystem" Source="Libraries.brsystem.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="astime" Source="Libraries.astime.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="asstring" Source="Libraries.asstring.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="AsIO" Source="Libraries.AsIO.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="AsArLog" Source="Libraries.AsArLog.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="AsARCfg" Source="Libraries.AsARCfg.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="Motorky" Source="Libraries.Motorky.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="DRV_mbus" Source="Libraries.DRV_mbus.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="LoopConR" Source="Libraries.LoopConR.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="SimpFun" Source="Libraries.SimpFun.lby" Memory="UserROM" Language="IEC" Debugging="true" />
    <LibraryObject Name="HydCtrl" Source="Libraries.HydCtrl.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="NcGlobal" Source="Libraries.NcGlobal.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="Acp10par" Source="Libraries.Acp10par.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="Acp10_MC" Source="Libraries.Acp10_MC.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="AsEPL" Source="Libraries.AsEPL.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="powerlnk" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="vcresman" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="ashydcon" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
  </Libraries>
</SwConfiguration>