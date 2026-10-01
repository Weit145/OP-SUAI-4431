package info.stepanoff.trsis.samples.db.dao;

import info.stepanoff.trsis.samples.db.model.UserPE;
import java.util.Optional;
import org.springframework.data.jpa.repository.JpaRepository;

public interface UserRepository extends JpaRepository<UserPE, Long> {
    Optional<UserPE> findByLogin(String login);
}
