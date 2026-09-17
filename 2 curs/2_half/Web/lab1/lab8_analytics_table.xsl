<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="html" encoding="UTF-8"/>

    <xsl:template match="/">
        <div class="xslt-result">
            <h2>Табличное отображение XML-файла</h2>
            <p>Данные отсортированы по рейтингу инструмента. Используются xsl:apply-templates, xsl:sort и xsl:if.</p>
            <table class="xml-table lab8-generated-table">
                <thead>
                    <tr>
                        <th>Графика</th>
                        <th>Название</th>
                        <th>Категория</th>
                        <th>Назначение</th>
                        <th>Тип данных</th>
                        <th>Формат</th>
                        <th>Рейтинг</th>
                        <th>Примечание</th>
                    </tr>
                </thead>
                <tbody>
                    <xsl:apply-templates select="analyticsCatalog/tool">
                        <xsl:sort select="@rating" data-type="number" order="descending"/>
                    </xsl:apply-templates>
                </tbody>
            </table>
        </div>
    </xsl:template>

    <xsl:template match="tool">
        <xsl:if test="@visible='true'">
            <tr>
                <td><img class="xml-icon" src="{image/@src}" alt="{image/@alt}"/></td>
                <td><strong><xsl:value-of select="name"/></strong><br/><small>ID: <xsl:value-of select="@id"/></small></td>
                <td><xsl:value-of select="@category"/><br/><small>Уровень: <xsl:value-of select="@level"/></small></td>
                <td><xsl:value-of select="purpose"/></td>
                <td><xsl:value-of select="dataType"/></td>
                <td><xsl:value-of select="format"/></td>
                <td><xsl:value-of select="@rating"/></td>
                <td><xsl:value-of select="note"/></td>
            </tr>
        </xsl:if>
    </xsl:template>
</xsl:stylesheet>
