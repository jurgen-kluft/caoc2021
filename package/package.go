package xaoc2021

import (
	xbase "github.com/jurgen-kluft/xbase/package"
	"github.com/jurgen-kluft/xcode/denv"
	xentry "github.com/jurgen-kluft/xentry/package"
	xfile "github.com/jurgen-kluft/xfile/package"
	xgenerics "github.com/jurgen-kluft/xgenerics/package"
	xtext "github.com/jurgen-kluft/xtext/package"
	xunittest "github.com/jurgen-kluft/xunittest/package"
)

// GetPackage returns the package object of 'xaoc2021'
func GetPackage() *denv.Package {
	// Dependencies
	xunittestpkg := xunittest.GetPackage()
	xentrypkg := xentry.GetPackage()
	xtextpkg := xtext.GetPackage()
	xfilepkg := xfile.GetPackage()
	xgenericspkg := xgenerics.GetPackage()
	xbasepkg := xbase.GetPackage()

	// The main (xtext) package
	mainpkg := denv.NewPackage("xtext")
	mainpkg.AddPackage(xunittestpkg)
	mainpkg.AddPackage(xentrypkg)
	mainpkg.AddPackage(xtextpkg)
	mainpkg.AddPackage(xfilepkg)
	mainpkg.AddPackage(xgenericspkg)
	mainpkg.AddPackage(xbasepkg)

	// 'xaoc2021' library
	mainlib := denv.SetupDefaultCppLibProject("xaoc2021", "github.com\\jurgen-kluft\\xaoc2021")
	mainlib.Dependencies = append(mainlib.Dependencies, xtextpkg.GetMainLib())
	mainlib.Dependencies = append(mainlib.Dependencies, xfilepkg.GetMainLib())
	mainlib.Dependencies = append(mainlib.Dependencies, xgenericspkg.GetMainLib())
	mainlib.Dependencies = append(mainlib.Dependencies, xbasepkg.GetMainLib())

	// 'xaoc2021' unittest project
	maintest := denv.SetupDefaultCppTestProject("xaoc2021_test", "github.com\\jurgen-kluft\\xaoc2021")
	maintest.Dependencies = append(maintest.Dependencies, xunittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, xentrypkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, xtextpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, xfilepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, xgenericspkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, xbasepkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
