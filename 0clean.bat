set RM=linux_tools\rm -rf

%RM% fix_name\*.db
%RM% fix_name\*.bin
%RM% fix_name\fix_name\*.bin
%RM% fix_name\Debug
%RM% fix_name\fix_name\Debug

%RM% fix_name_v2\*.db
%RM% fix_name_v2\*.bin
%RM% fix_name_v2\fix_name\*.bin
%RM% fix_name_v2\Debug
%RM% fix_name_v2\fix_name\Debug

%RM% jlink_dcc_view\Debug\*
%RM% jlink_dcc_view\jlink_dcc\Debug\*

%RM% wcdma_m_emu/ipch
%RM% wcdma_m_emu/*.db
%RM% wcdma_m_emu/out/Log*
%RM% wcdma_m_emu/out/*.txt
%RM% wcdma_m_emu/out/*.lib
%RM% wcdma_m_emu/out/*.exe
%RM% wcdma_m_emu/out/*.pdb
%RM% wcdma_m_emu/out/*.exp

%RM% wcdma_m_emu/hxTestTool/obj
%RM% wcdma_m_emu/hxTestTool/bin

%RM% wcdma_m_emu/hxSerialProtocol/Debug

pause