<!-- run with   xsltproc gpx2gccode.xsl GPX_FILE -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"  xmlns:g="http://www.topografix.com/GPX/1/0">
	<xsl:output method="text"/>
	<xsl:template match="g:gpx">
		<xsl:for-each select="g:wpt">
			<xsl:value-of select="g:name"/>
			<xsl:text>,</xsl:text>
		</xsl:for-each>
	</xsl:template>
</xsl:stylesheet>
