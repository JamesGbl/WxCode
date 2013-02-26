project.name = "wxReportDocument"

-- Configurations. All I am doing is reordering them so that
-- it defaults to a Release build.
project.configs = { "Release", "Debug" }

-- Add packages here.
dopackage( "../src" )
dopackage( "../samples/LayoutGeneratorGUI" )
dopackage( "../samples/TableDemo" )
dopackage( "../samples/TemplateViewer" )

