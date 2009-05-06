package.name = "Premake.Tests"
package.kind = "dll"
package.language = "c#"

-- Adding this for GNU generator on Windows; how does VS.NET find it?

if (windows) then
	package.libpaths = { "C:/Program Files/NUnit 2.2/bin" }
end

package.links =
{
	"System",
	"nunit.framework"
}

package.files =
{
	matchrecursive("*.cs")
}
