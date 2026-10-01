package info.stepanoff.trsis.samples.rest;

import info.stepanoff.trsis.samples.rest.model.PropertyDTO;
import info.stepanoff.trsis.samples.service.PropertyService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import java.net.URI;
import java.util.List;
import lombok.RequiredArgsConstructor;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.servlet.support.ServletUriComponentsBuilder;

@RestController
@RequestMapping("/api/properties")
@RequiredArgsConstructor
@Tag(name = "Недвижимость", description = "CRUD-операции с объектами аренды")
public class PropertyRestController {

    private final PropertyService propertyService;

    @GetMapping
    @Operation(summary = "Получить все объекты")
    @ApiResponse(responseCode = "200", description = "Список получен")
    public ResponseEntity<List<PropertyDTO>> findAll() {
        return ResponseEntity.ok(propertyService.findAll());
    }

    @GetMapping("/{id}")
    @Operation(summary = "Получить объект по идентификатору")
    @ApiResponse(responseCode = "200", description = "Объект найден")
    @ApiResponse(responseCode = "404", description = "Объект не найден", content = @Content(schema = @Schema(implementation = ApiError.class)))
    public ResponseEntity<PropertyDTO> findById(
            @Parameter(description = "Идентификатор", example = "1") @PathVariable Long id) {
        return ResponseEntity.ok(propertyService.findById(id));
    }

    @PostMapping
    @Operation(summary = "Добавить объект")
    @ApiResponse(responseCode = "201", description = "Объект создан")
    @ApiResponse(responseCode = "400", description = "Некорректные данные", content = @Content(schema = @Schema(implementation = ApiError.class)))
    @ApiResponse(responseCode = "401", description = "Требуется вход в систему")
    @ApiResponse(responseCode = "403", description = "Нет прав или неверный CSRF-токен")
    public ResponseEntity<PropertyDTO> create(@Valid @RequestBody PropertyDTO request) {
        PropertyDTO created = propertyService.create(request);
        URI location = ServletUriComponentsBuilder.fromCurrentRequest().path("/{id}")
                .buildAndExpand(created.getId()).toUri();
        return ResponseEntity.created(location).body(created);
    }

    @PutMapping("/{id}")
    @Operation(summary = "Полностью обновить объект")
    @ApiResponse(responseCode = "200", description = "Объект обновлён")
    @ApiResponse(responseCode = "400", description = "Некорректные данные", content = @Content(schema = @Schema(implementation = ApiError.class)))
    @ApiResponse(responseCode = "404", description = "Объект не найден", content = @Content(schema = @Schema(implementation = ApiError.class)))
    @ApiResponse(responseCode = "401", description = "Требуется вход в систему")
    @ApiResponse(responseCode = "403", description = "Нет прав или неверный CSRF-токен")
    public ResponseEntity<PropertyDTO> update(@PathVariable Long id,
                                               @Valid @RequestBody PropertyDTO request) {
        return ResponseEntity.ok(propertyService.update(id, request));
    }

    @DeleteMapping("/{id}")
    @Operation(summary = "Удалить объект")
    @ApiResponse(responseCode = "204", description = "Объект удалён")
    @ApiResponse(responseCode = "404", description = "Объект не найден", content = @Content(schema = @Schema(implementation = ApiError.class)))
    @ApiResponse(responseCode = "401", description = "Требуется вход в систему")
    @ApiResponse(responseCode = "403", description = "Нет прав или неверный CSRF-токен")
    public ResponseEntity<Void> delete(@PathVariable Long id) {
        propertyService.delete(id);
        return ResponseEntity.noContent().build();
    }
}
