package info.stepanoff.trsis.samples.rest.model;

import info.stepanoff.trsis.samples.db.model.PropertyType;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.DecimalMin;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import java.math.BigDecimal;
import lombok.Data;

@Data
@Schema(description = "Объект недвижимости, доступный для аренды")
public class PropertyDTO {

    @Schema(description = "Идентификатор", accessMode = Schema.AccessMode.READ_ONLY, example = "1")
    private Long id;

    @NotBlank
    @Size(max = 255)
    @Schema(description = "Адрес", example = "Санкт-Петербург, Литейный проспект, 10")
    private String address;

    @NotNull
    @Schema(description = "Тип объекта", example = "APARTMENT")
    private PropertyType type;

    @NotNull
    @DecimalMin(value = "0.1")
    @Schema(description = "Площадь, м²", example = "42.5")
    private BigDecimal area;

    @NotNull
    @Min(1)
    @Schema(description = "Число комнат", example = "2")
    private Integer rooms;

    @NotNull
    @DecimalMin(value = "0.0", inclusive = false)
    @Schema(description = "Арендная плата в месяц, руб.", example = "55000")
    private BigDecimal monthlyRent;

    @NotNull
    @Schema(description = "Доступен ли объект для аренды", example = "true")
    private Boolean available;
}
