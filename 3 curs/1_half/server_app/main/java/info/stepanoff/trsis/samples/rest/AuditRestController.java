package info.stepanoff.trsis.samples.rest;

import info.stepanoff.trsis.samples.db.dao.AuditEventRepository;
import info.stepanoff.trsis.samples.db.model.AuditEventPE;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import java.util.List;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/api/audit")
@RequiredArgsConstructor
public class AuditRestController {
    private final AuditEventRepository auditEventRepository;

    @GetMapping
    @Operation(summary = "Журнал изменений объектов недвижимости")
    @ApiResponse(responseCode = "200", description = "Журнал получен")
    @ApiResponse(responseCode = "401", description = "Требуется вход в систему")
    @ApiResponse(responseCode = "403", description = "Недостаточно прав")
    public List<AuditEventPE> list() {
        return auditEventRepository.findAllByOrderByIdDesc();
    }
}
