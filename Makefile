#############################################################################
# Makefile for building: LMP
# Generated by qmake (2.01a) (Qt 4.8.4) on: Wed Jun 19 12:00:17 2013
# Project:  LMP.pro
# Template: app
# Command: c:\Qt\4.8.4\bin\qmake.exe -config release -o Makefile LMP.pro
#############################################################################

first: release
install: release-install
uninstall: release-uninstall
MAKEFILE      = Makefile
QMAKE         = c:\Qt\4.8.4\bin\qmake.exe
DEL_FILE      = del
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
COPY          = copy /y
COPY_FILE     = $(COPY)
COPY_DIR      = xcopy /s /q /y /i
INSTALL_FILE  = $(COPY_FILE)
INSTALL_PROGRAM = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = del
SYMLINK       = 
DEL_DIR       = rmdir
MOVE          = move
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
SUBTARGETS    =  \
		release \
		debug

release: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release
release-make_default: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release 
release-make_first: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release first
release-all: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release all
release-clean: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release clean
release-distclean: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release distclean
release-install: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release install
release-uninstall: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release uninstall
debug: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug
debug-make_default: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug 
debug-make_first: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug first
debug-all: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug all
debug-clean: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug clean
debug-distclean: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug distclean
debug-install: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug install
debug-uninstall: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug uninstall

Makefile: LMP.pro  ..\..\..\..\Qt\4.8.4\mkspecs\win32-msvc2010\qmake.conf ..\..\..\..\Qt\4.8.4\mkspecs\qconfig.pri \
		..\..\..\..\Qt\4.8.4\mkspecs\modules\qt_webkit_version.pri \
		..\..\..\..\Qt\4.8.4\mkspecs\features\qt_functions.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\qt_config.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\exclusive_builds.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\default_pre.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\win32\default_pre.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\release.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\debug_and_release.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\default_post.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\win32\default_post.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\win32\rtti.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\win32\exceptions.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\win32\stl.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\shared.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\win32\embed_manifest_exe.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\win32\embed_manifest_dll.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\warn_on.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\qt.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\win32\thread.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\moc.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\win32\windows.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\resources.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\uic.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\yacc.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\lex.prf \
		..\..\..\..\Qt\4.8.4\mkspecs\features\include_source_dir.prf \
		c:\Qt\4.8.4\lib\qtmain.prl
	$(QMAKE) -config release -o Makefile LMP.pro
..\..\..\..\Qt\4.8.4\mkspecs\qconfig.pri:
..\..\..\..\Qt\4.8.4\mkspecs\modules\qt_webkit_version.pri:
..\..\..\..\Qt\4.8.4\mkspecs\features\qt_functions.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\qt_config.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\exclusive_builds.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\default_pre.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\win32\default_pre.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\release.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\debug_and_release.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\default_post.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\win32\default_post.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\win32\rtti.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\win32\exceptions.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\win32\stl.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\shared.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\win32\embed_manifest_exe.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\win32\embed_manifest_dll.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\warn_on.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\qt.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\win32\thread.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\moc.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\win32\windows.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\resources.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\uic.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\yacc.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\lex.prf:
..\..\..\..\Qt\4.8.4\mkspecs\features\include_source_dir.prf:
c:\Qt\4.8.4\lib\qtmain.prl:
qmake: qmake_all FORCE
	@$(QMAKE) -config release -o Makefile LMP.pro

qmake_all: FORCE

make_default: release-make_default debug-make_default FORCE
make_first: release-make_first debug-make_first FORCE
all: release-all debug-all FORCE
clean: release-clean debug-clean FORCE
	-$(DEL_FILE) ".\LMP.intermediate.manifest"
	-$(DEL_FILE) LMP.exp
distclean: release-distclean debug-distclean FORCE
	-$(DEL_FILE) Makefile

check: first

release-mocclean: $(MAKEFILE).Release
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release mocclean
debug-mocclean: $(MAKEFILE).Debug
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug mocclean
mocclean: release-mocclean debug-mocclean

release-mocables: $(MAKEFILE).Release
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release mocables
debug-mocables: $(MAKEFILE).Debug
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug mocables
mocables: release-mocables debug-mocables
FORCE:

$(MAKEFILE).Release: Makefile
$(MAKEFILE).Debug: Makefile
