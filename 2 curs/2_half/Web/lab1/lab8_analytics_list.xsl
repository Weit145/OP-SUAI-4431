<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="html" encoding="UTF-8"/>

    <xsl:template match="/">
        <div class="xslt-result">
            <h2>Построчное отображение XML-файла</h2>
            <p>Каждый элемент XML выводится отдельной карточкой.</p>
            <div class="xml-card-list">
                <xsl:apply-templates select="analyticsCatalog/tool">
                    <xsl:sort select="name" order="ascending"/>
                </xsl:apply-templates>
            </div>
        </div>
    </xsl:template>

    <xsl:template match="tool">
        <xsl:if test="@visible='true'">
            <article class="xml-card">
                <img class="xml-icon" src="{image/@src}" alt="{image/@alt}"/>
                <div>
                    <h3><xsl:value-of select="name"/> — <xsl:value-of select="@category"/></h3>
                    <p><strong>Назначение:</strong> <xsl:value-of select="purpose"/></p>
                    <p><strong>Данные:</strong> <xsl:value-of select="dataType"/>. <strong>Формат:</strong> <xsl:value-of select="format"/>.</p>
                    <p><strong>Уровень:</strong> <xsl:value-of select="@level"/>. <strong>Рейтинг:</strong> <xsl:value-of select="@rating"/>.</p>
                    <p><xsl:value-of select="note"/></p>
                </div>
            </article>
        </xsl:if>
    </xsl:template>
</xsl:stylesheet>
