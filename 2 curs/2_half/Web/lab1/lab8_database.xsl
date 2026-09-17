<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="html" encoding="UTF-8"/>

    <xsl:template match="/">
        <div class="xslt-result">
            <h2>XML-файл на основе базы данных лабораторной работы №6</h2>
            <p>Данные представлены с вложением дочерней таблицы Project в родительский элемент Direction.</p>
            <xsl:apply-templates select="NewDataSet/Direction">
                <xsl:sort select="direction_name" order="ascending"/>
            </xsl:apply-templates>
        </div>
    </xsl:template>

    <xsl:template match="Direction">
        <section class="xml-db-block">
            <h3><xsl:value-of select="direction_name"/> <small>(ID: <xsl:value-of select="@direction_id"/>)</small></h3>
            <p><strong>Источник:</strong> <xsl:value-of select="data_source"/>. <strong>Метод:</strong> <xsl:value-of select="main_method"/>.</p>
            <p><strong>Цель:</strong> <xsl:value-of select="business_goal"/>. <strong>Сложность:</strong> <xsl:value-of select="difficulty_level"/>.</p>
            <xsl:if test="Project">
                <table class="xml-table lab8-generated-table">
                    <thead>
                        <tr>
                            <th>Проект</th>
                            <th>Метрика</th>
                            <th>Инструмент</th>
                            <th>Размер набора</th>
                            <th>Результат</th>
                            <th>Ответственный</th>
                        </tr>
                    </thead>
                    <tbody>
                        <xsl:apply-templates select="Project">
                            <xsl:sort select="project_name" order="ascending"/>
                        </xsl:apply-templates>
                    </tbody>
                </table>
            </xsl:if>
        </section>
    </xsl:template>

    <xsl:template match="Project">
        <tr>
            <td><xsl:value-of select="project_name"/> <br/><small>ID: <xsl:value-of select="@project_id"/></small></td>
            <td><xsl:value-of select="metric_name"/></td>
            <td><xsl:value-of select="tool_name"/></td>
            <td><xsl:value-of select="dataset_size"/></td>
            <td><xsl:value-of select="result_summary"/></td>
            <td><xsl:value-of select="responsible"/></td>
        </tr>
    </xsl:template>
</xsl:stylesheet>
