
Optionsps.dll: dlldata.obj Options_p.obj Options_i.obj
	link /dll /out:Optionsps.dll /def:Optionsps.def /entry:DllMain dlldata.obj Options_p.obj Options_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del Optionsps.dll
	@del Optionsps.lib
	@del Optionsps.exp
	@del dlldata.obj
	@del Options_p.obj
	@del Options_i.obj
