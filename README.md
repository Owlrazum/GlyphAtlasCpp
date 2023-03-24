# Description

The project uses cmake definition PROJECT_SOURCE_DIR to determine paths.
The included font files are referenced by the test data, no APIs for font location are in place.

### Windows

The freetype library for GlyphAtlas on Windows `freetype.lib` was built using:<br>
`freetype-2.13.0/builds/windows/vc2010/freetype.sln` in `Release Static` configuration.
The included files in this project should be from there.

To address warning `LNK4098: defaultlib 'LIBCMT' conflicts with use of other libs` <br>
the link options `/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib /NODEFAULTLIB:msvcrtd.lib` were used

### Mac OS / Unix

I have forgotten how The freetype library for GlyphAtlas on Mac OS
`libfreetype.a` and `libfreetyped.a` <br>
were built , will update this section as I find out or rebuild.