package info.stepanoff.trsis.samples.db.model;

import io.swagger.v3.oas.annotations.media.Schema;

@Schema(description = "Тип объекта недвижимости")
public enum PropertyType {
    APARTMENT,
    HOUSE,
    ROOM,
    COMMERCIAL
}
