<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio Version=4.2.12.129 SP?>
<SwConfiguration CpuAddress="SL1" xmlns="http://br-automation.co.at/AS/SwConfiguration">
  <TaskClass Name="Cyclic#1">
    <Task Name="tk1_in" Source="In_Out.tk1_in.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="timer" Source="process.timer.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="clamp" Source="process.clamp.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="carriage" Source="process.carriage.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="blowpin" Source="process.blowpin.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="blowpin2" Source="process.blowpin2.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="extrlift" Source="process.extrlift.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="mc_pwr" Source="Motion.mc_pwr.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="tk1_out" Source="In_Out.tk1_out.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="parison" Source="thickness.parison.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="robot" Source="process.robot.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="r_carriage" Source="Motion.r_carriage.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="l_carriage" Source="Motion.l_carriage.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="r_blowpin" Source="Motion.r_blowpin.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="l_blowpin" Source="Motion.l_blowpin.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="r_blowpin2" Source="Motion.r_blowpin2.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="l_blowpin2" Source="Motion.l_blowpin2.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="l_clamp" Source="Motion.l_clamp.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="r_clamp" Source="Motion.r_clamp.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="l_transfer" Source="Motion.l_transfer.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="r_transfer" Source="Motion.r_transfer.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="mc_extrlif" Source="Motion.mc_extrlift.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="mc_thick" Source="Motion.mc_thick.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="Trace" Source="screen.Trace.prg" Memory="UserROM" Language="IEC" Debugging="true" />
    <Task Name="clean_Test" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#2">
    <Task Name="h_pwm" Source="Heating.h_pwm.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="tk2_io" Source="In_Out.tk2_io.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#3">
    <Task Name="cutter" Source="process.cutter.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="cutneck" Source="process.cutneck.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="cooldef" Source="process.cooldef.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="blowdef" Source="process.blowdef.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="bdeflash" Source="process.bdeflash.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="tdeflash" Source="process.tdeflash.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="tdeflashop" Source="process.tdeflashopncls.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="pullbottom" Source="process.pullbottom.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="inerpin" Source="process.inerpin.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="highblow" Source="process.highblow.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="lowblow" Source="process.lowblow.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="hookpin" Source="process.hookpin.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="coolpin" Source="process.coolpin.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="coolpin2" Source="process.coolpin2.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="coolpin3" Source="process.coolpin3.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="coolblw" Source="process.coolblw.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="coolblw2" Source="process.coolblw2.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="coolblw3" Source="process.coolblw3.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="robotopncl" Source="process.robotopncls.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="submold" Source="process.submold.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="pinpart" Source="process.pinpart.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="screw" Source="process.screw.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="sealer" Source="process.sealer.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="bblowpin" Source="process.bblowpin.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="support" Source="process.support.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="tonnage" Source="process.tonnage.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="autorun" Source="process.autorun.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="extru_plk" Source="machine.extru_plk.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="extruder" Source="machine.extruder.prg" Memory="UserROM" Language="ANSIC" Debugging="true" Disabled="true" />
    <Task Name="extruder_M" Source="machine.extruder_Mix.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="Acs580Ctl" Source="machine.Acs580Ctl.prg" Memory="UserROM" Language="IEC" Debugging="true" />
    <Task Name="plate" Source="process.plate.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
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
    <Task Name="clean" Source="machine.clean.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="length" Source="process.length.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#5">
    <Task Name="h_pid" Source="Heating.h_pid.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#6">
    <Task Name="cur_cal" Source="thickness.cur_cal.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="exchange" Source="thickness.exchange.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <TaskClass Name="Cyclic#7">
    <Task Name="cur_edit" Source="thickness.cur_edit.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="cur_draw" Source="thickness.cur_draw.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="pvchg" Source="VarChgLog.pvchg.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
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
    <Task Name="cur_save" Source="thickness.cur_save.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="cfg_cpy" Source="recipeMan.cfg_cpy.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="alarm" Source="machine.alarm.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="energy" Source="machine.energy.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <Task Name="epl" Source="comm.epl.prg" Memory="UserROM" Language="ANSIC" Debugging="true" />
  </TaskClass>
  <DataObjects>
    <DataObject Name="mb_cmd" Source="comm.mb_cmd.dob" Memory="UserROM" Language="Simple" />
    <DataObject Name="var_fix" Source="recipeMan.var_fix.dob" Memory="UserROM" Language="Simple" />
    <DataObject Name="var_mold" Source="recipeMan.var_mold.dob" Memory="UserROM" Language="Simple" />
    <DataObject Name="log_txt" Source="log_info.log_txt.dob" Memory="UserROM" Language="Simple" />
    <DataObject Name="pv_chg" Source="VarChgLog.pv_chg.dob" Memory="UserROM" Language="Simple" />
    <DataObject Name="cfg_dat" Source="recipeMan.cfg_dat.dob" Memory="UserROM" Language="Simple" />
    <DataObject Name="Acp10sys" Source="" Memory="UserROM" Language="Binary" />
    <DataObject Name="recName" Source="" Memory="UserROM" Language="Binary" />
    <DataObject Name="recFix" Source="" Memory="UserROM" Language="Binary" />
    <DataObject Name="recCurve" Source="" Memory="UserROM" Language="Binary" />
    <DataObject Name="recMold" Source="" Memory="UserROM" Language="Binary" />
  </DataObjects>
  <NcDataObjects>
    <NcDataObject Name="TransferR" Source="Motion.init.TransferR.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="CarriageR" Source="Motion.init.CarriageR.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="CarriageL" Source="Motion.init.CarriageL.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="BlowPinR" Source="Motion.init.BlowPinR.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="BlowPinL" Source="Motion.init.BlowPinL.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="BlowPin2R" Source="Motion.init.BlowPin2R.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="BlowPin2L" Source="Motion.init.BlowPin2L.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="ThickInit" Source="Motion.init.ThickInit.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="Tansfer" Source="Motion.para.Tansfer.dob" Memory="UserROM" Language="Apt" />
    <NcDataObject Name="UpDn" Source="Motion.para.UpDn.dob" Memory="UserROM" Language="Apt" />
    <NcDataObject Name="CarraigeLR" Source="Motion.para.CarraigeLR.dob" Memory="UserROM" Language="Apt" />
    <NcDataObject Name="BlowPinLR" Source="Motion.para.BlowPinLR.dob" Memory="UserROM" Language="Apt" />
    <NcDataObject Name="nCarraigeL" Source="Motion.para.nCarraigeLR.dob" Memory="UserROM" Language="Apt" />
    <NcDataObject Name="nBlowPinLR" Source="Motion.para.nBlowPinLR.dob" Memory="UserROM" Language="Apt" />
    <NcDataObject Name="BlowPin2LR" Source="Motion.para.BlowPin2LR.dob" Memory="UserROM" Language="Apt" />
    <NcDataObject Name="ClampLR" Source="Motion.para.ClampLR.dob" Memory="UserROM" Language="Apt" />
    <NcDataObject Name="acp10etxen" Source="Motion.acp10etxen.dob" Memory="UserROM" Language="Ett" />
    <NcDataObject Name="Power" Source="Motion.init.Power.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="Power1" Source="Motion.para.Power.dob" Memory="UserROM" Language="Apt" />
    <NcDataObject Name="ClampR" Source="Motion.init.ClampR.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="ClampL" Source="Motion.init.ClampL.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="ExtrLift1" Source="Motion.init.ExtrLift.dob" Memory="UserROM" Language="Ax" />
    <NcDataObject Name="ExtrLift2" Source="Motion.para.ExtrLift.dob" Memory="UserROM" Language="Apt" />
    <NcDataObject Name="ThickPara" Source="Motion.para.ThickPara.dob" Memory="UserROM" Language="Apt" />
    <NcDataObject Name="0804Thick" Source="" Memory="UserROM" Language="Binary" />
    <NcDataObject Name="0129_C2" Source="" Memory="UserROM" Language="Binary" />
    <NcDataObject Name="gAxisPwri" Source="" Memory="UserROM" Language="Binary" />
    <NcDataObject Name="gAxisPwra" Source="" Memory="UserROM" Language="Binary" />
  </NcDataObjects>
  <VcDataObjects>
    <VcDataObject Name="visu15" Source="Visu.visu15.dob" Memory="UserROM" Language="Vc" WarningLevel="2" Compress="false" />
  </VcDataObjects>
  <Binaries>
    <BinaryObject Name="TCData" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="vctcal" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccnum" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccalarm" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccovl" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcshared" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="visu1502" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcfile" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccstr" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccline" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arial" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcctrend" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcchtml" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcgclass" Source="" Memory="UserROM" Language="Binary" />
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
    <BinaryObject Name="visu1501" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccshape" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcchspot" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcmgr" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcdsloc" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="visvc" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccurl" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccdt" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccbar" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpdsw" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcxml" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccscale" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcbclass" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcnet" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpdvnc" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccddbox" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccpopup" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="timesbd" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="times" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arialxsr" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vccslider" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpfmtcx" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpk" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpfapc" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcptelo" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpdi815" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpdi855" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpdihd" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="sysconf" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="asfw" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="User" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="iomap" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arconfig" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="ashwd" Source="" Memory="SystemROM" Language="Binary" />
    <BinaryObject Name="Role" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="acp10cfg" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="Acp10map" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpfar00" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="vcpkat" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="tahomabd" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="arialbd" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="tahoma" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="myxh_1" Source="" Memory="UserROM" Language="Binary" />
    <BinaryObject Name="myxh" Source="" Memory="UserROM" Language="Binary" />
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
    <LibraryObject Name="AsHydCon" Source="Libraries.AsHydCon.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="AsEPL" Source="Libraries.AsEPL.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="Acp10man" Source="Libraries.Acp10man.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="Acp10sim" Source="Libraries.Acp10sim.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="vcscrsht" Source="Libraries.vcscrsht.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="powerlnk" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="vcresman" Source="" Memory="UserROM" Language="Binary" Debugging="true" />
  </Libraries>
</SwConfiguration>